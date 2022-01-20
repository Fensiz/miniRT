#include "minirt.h"
#include <mlx.h>
#include <math.h>
#include <stdio.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (dlen >= dstsize)
		return (dstsize + slen);
	dst += dlen;
	i = dlen + 1;
	while (*src && i++ < dstsize)
		*dst++ = *src++;
	*dst = 0;
	return (dlen + slen);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize && src && dst)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	return (ft_strlen(src));
}
void		ft_usage_message(const char *program_name)
{
	write(1, "Usage: ", 7);
	write(1, program_name, ft_strlen(program_name));
	write(1, " <scene.rt>\n", 11);
	exit(EXIT_FAILURE);
}
void	ft_error(int code, char *error_text)
{
	if (code == SCENE)
		write(2, "Scene error: ", 13);
	else if (code == FATAL)
		write(2, "Fatal error: ", 13);
//	else if (code == FIG_TYPE)
//		write(2, "Figure error: ", 14);
	else if (code == FILE_FORMAT_ERR)
		write(2, "Wrong file format", 17);
	write(2, error_text, ft_strlen(error_text));
	write(2, "\n", 1);
	exit(code);
}
static t_figure	*ft_newfigure(void)
{
	t_figure	*new;

	new = memalloc(sizeof(t_figure));
	new->next = NULL;
	return (new);
}
t_figure	*ft_addback_figure(t_figure **lst)
{
	t_figure	*curr;

	curr = *lst;
	if (!curr)
	{
		*lst = ft_newfigure();
		curr = *lst;
	}
	else
	{
		while (curr->next)
			curr = curr->next;
		curr->next = ft_newfigure();
		curr = curr->next;
	}
	return (curr);
}
void	check_value(double value, double min, double max, char *name)
{
	char	error[100];

	if (value < min || value > max)
	{
		ft_strlcpy(error, name, 81);
		ft_strlcat(error, " value out of range", 19);
		ft_error(SCENE, error);
	}
}
int	parse_color(char **str)
{
	int	red;
	int	green;
	int	blue;

	red = 0;
	green = 0;
	blue = 0;
	red = ft_atoi(str);
	check_value(red, 0, 255, "color");
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	green = ft_atoi(str);
	check_value(green, 0, 255, "color");
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	blue = ft_atoi(str);
	check_value(blue, 0, 255, "color");
	return ((red << 16) + (green << 8) + blue);
}
void		parse_ambient_light(t_scene *scene, char **str)
{
	(*str)++;
	skip_spaces(str);
	scene->ambient_light = ft_atof(str);
	check_value(scene->ambient_light, 0, 1, "ambient lightning");
	scene->ambient_light_color = parse_color(str);
}

t_vector	parse_vector(char **str)
{
	t_vector	v;

	v.x = ft_atof(str);
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	v.y = ft_atof(str);
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	v.z = ft_atof(str);
	return (v);
}
void		parse_camera(t_mlx *mlx, t_scene *scene, char **str)
{
	t_camera	*new;
	t_camera	*curr;
	int			prev_cam_id;

	(*str)++;
	prev_cam_id = 0;
	curr = mlx->camera;
	new = memalloc(sizeof(t_camera));
	new->next = NULL;
	if (curr)
	{
		while (curr->next)
			curr = curr->next;
		prev_cam_id = curr->cam_id;
		curr->next = new;
	}
	else
		mlx->camera = new;
	skip_spaces(str);
	new->cam_id = prev_cam_id + 1;
	scene->cam_count = new->cam_id;
	new->origin = parse_vector(str);
	new->direction = vector_norm(parse_vector(str));
	new->fov = ft_atoi(str);
	check_value(new->fov, 0, 180, "camera");
}

void		parse_light(t_scene **scene, char **str)
{
	t_light	*new;
	t_light	*curr;

	(*str)++;
	curr = (*scene)->light;
	new = memalloc(sizeof(t_light));
	new->next = NULL;
	if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	else
		(*scene)->light = new;
	skip_spaces(str);
	new->origin = parse_vector(str);
	new->brightness = ft_atof(str);
	check_value(new->brightness, 0, 1, "light");
	new->color = parse_color(str);
}

void		parse_sphere(t_figure **figure_list, char **str)
{
	t_figure	*curr;

	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = SPHERE;
	skip_spaces(str);
	curr->figure.sp.center = parse_vector(str);
	curr->figure.sp.radius = ft_atof(str) / 2;
	check_value(curr->figure.sp.radius, 0, INFINITY, "sphere");
	curr->color = parse_color(str);
	/* bonus */
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, "sphere");
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, "sphere");
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 5, "sphere");
}

void		parse_plane(t_figure **figure_list, char **str)
{
	t_figure	*curr;

	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = PLANE;
	skip_spaces(str);
	curr->figure.pl.point = parse_vector(str);
	curr->normal = vector_norm(parse_vector(str));
	curr->color = parse_color(str);
	/* bonus */
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, "plane");
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, "plane");
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 5, "plane");
}

void		parse_cylinder(t_figure **figure_list, char **str)
{
	t_figure	*curr;

	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = CYLINDER;
	skip_spaces(str);
	curr->figure.cy.center = parse_vector(str);
	curr->figure.cy.nv = vector_norm(parse_vector(str));
	curr->figure.cy.radius = ft_atof(str) / 2;
	check_value(curr->figure.cy.radius, 0, INFINITY, "cylinder");
	curr->figure.cy.height = ft_atof(str);
	check_value(curr->figure.cy.height, 0, INFINITY, "cylinder");
	curr->color = parse_color(str);
	/* bonus */
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, "cylinder");
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, "cylinder");
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 5, "cylinder");
}

void		parse_cone(t_figure **figure_list, char **str)
{
	t_figure	*curr;

	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = CONE;
	skip_spaces(str);
	curr->figure.co.center = parse_vector(str);
	curr->figure.co.nv = vector_norm(parse_vector(str));
	curr->figure.co.radius = ft_atof(str) / 2;
	check_value(curr->figure.co.radius, 0, INFINITY, "cone");
	curr->figure.co.height = ft_atof(str);
	check_value(curr->figure.co.height, 0, INFINITY, "cone");
	curr->color = parse_color(str);
	/* bonus */
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, "cone");
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, "cone");
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 5, "cone");
}

static void	parse(t_mlx *mlx, t_scene *scene, t_figure **figure, char **s)
{
	if (**s == 'A')
		parse_ambient_light(scene, s);
	else if (**s == 'C')
		parse_camera(mlx, scene, s);
	else if (**s == 'c' && *(*s + 1) == 'y')
		parse_cylinder(figure, s);
	else if (**s == 'c' && *(*s + 1) == 'o')
		parse_cone(figure, s);
	else if (**s == 'L')
		parse_light(&scene, s);
	else if (**s == 's' && *(*s + 1) == 'p')
		parse_sphere(figure, s);
	else if (**s == 'p' && *(*s + 1) == 'l')
		parse_plane(figure, s);
	else
		ft_error(SCENE, " parse error: undefined object in the input file");
}

static void	parse_elems(t_mlx *mlx, t_scene *scene, t_figure **figure, char *str)
{
	while (*str)
	{
		while (ft_isspace(*str) || *str == '\n' || *str == '#') {
			if (ft_isspace(*str))
				skip_spaces(&str);
			else if (*str == '\n')
				str ++;
			else
			{
				while (*str && *str != '\n')
					str ++;
			}
		}
		if (!*str)
			break ;
		parse(mlx, scene, figure, &str);
		str++;
	}
	if (mlx->camera == NULL)
		ft_error(SCENE, " parse error: no cam");
}

char	*file_to_str(int fd)
{
	char	buffer[BUFF_SIZE + 1];
	char	*str;
	char	*tmp;
	size_t	r_size;

	str = NULL;
	r_size = read(fd, buffer, BUFF_SIZE);
	while (r_size)
	{
		tmp = str;
		buffer[r_size] = '\0';
		str = ft_strjoin(str, buffer);
		free(tmp);
		r_size = read(fd, buffer, BUFF_SIZE);
	}
	return (str);
}

int	is_right_format(const char *fname)
{
	int len;

	len = (int)ft_strlen(fname);
	if (len <= 3)
		return (0);
	if (!(fname[len - 1] == 't' && fname[len - 2] == 'r' && fname[len - 3] == '.'))
		return (0);
	return (1);
}

void	parse_scene(t_mlx *mlx, t_scene *scene, t_figure **figure,
							const char **argv)
{
	char		*str;
	int			fd;

	*figure = NULL;
	scene->light = NULL;
	mlx->camera = NULL;
	fd = open(argv[1], O_RDONLY);
	if (!is_right_format(argv[1]))
		ft_error(FILE_FORMAT_ERR, "");
	if (fd == -1)
		ft_fatal(FD);
	str = file_to_str(fd);
	parse_elems(mlx, scene, figure, str);
	free(str);
}

void	my_put_pixel(t_mlx *mlx, int x, int y, int color)
{
	char	*dst;

	dst = mlx->camera->img_addr + (y * mlx->camera->line_length + x * (mlx->camera->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
#include <stdio.h>
static t_vector		canvas_to_viewport(double x, double y, t_camera *camera, t_scene *scene)
{
	double		aspect_ratio;
	double		new_fov;
	double		vplane_width;
	double		vplane_height;
	t_vector	v;

	x = x - scene->width / 2;
	y = scene->height / 2 - y;
	aspect_ratio = (double)scene->width / (double)scene->height;
	new_fov = tan(camera->fov / 2 * (M_PI / 180));
	vplane_width = 2 * new_fov;
	vplane_height = vplane_width / aspect_ratio;
	v.x = x * vplane_width / scene->width;
	v.y = y * vplane_height / scene->height;
	v.z = 1;
	v = vector_norm(v);
	return (v);
}

static t_vector		cam_rot(t_vector d, t_vector cam_direction)
{
	t_vector	rotated;

	d = vector_x_rot(d, -90);
	rotated = rot_from_y1_to_n(d, cam_direction);
	return (rotated);
}

void		render_scene(t_scene *scene, t_figure *figure, t_mlx *mlx, t_camera *camera)
{
	int			color;
	int			x;
	int			y;
	t_vector_2p	ray;

	y = -1;
	ray.origin = camera->origin;
	while (++y < scene->height)
	{
		x = -1;
		while (++x < scene->width)
		{
			ray.direction = canvas_to_viewport(x, y, camera, scene);
			ray.direction = cam_rot(ray.direction, camera->direction);
			color = trace_ray(ray, BOUNCE_LIMIT, figure, scene);
			my_put_pixel(mlx, x, y, color);
		}
	}
}

void	get_closest_inter(t_vector_2p ray, t_figure *figure,
					t_figure *closest_figure, double *closest_inter)
{
	double distance;

	distance = INFINITY;
	while (figure)
	{
		if (figure->type == SPHERE)
			distance = sphere_intersection(ray, figure);
		else if (figure->type == PLANE)
			distance = plane_intersection(ray, figure);
		else if (figure->type == CYLINDER)
			distance = cylinder_intersection(ray, figure);
		else if (figure->type == CONE)
			distance = cone_intersection(ray, figure);
		if (distance > EPSILON && distance < *closest_inter)
		{
			*closest_figure = *figure;
			*closest_inter = distance;
		}
		figure = figure->next;
	}
}




double	calc_specular(t_vector_2p ray, t_inter *inter, t_scene scene, t_figure *lst)
{
	double		light;
	t_vector	direction;
	t_vector	to_cam;
	t_vector	reflected;

	direction = vector_sub(scene.light->origin, inter->point);
	to_cam = vector_sub(ray.origin, inter->point);
	reflected = reflect_ray(direction, inter->normal);
	if (vector_dot(reflected, to_cam) > 0)
		light = scene.light->brightness * pow(vector_cos(reflected, to_cam), lst->specular);
	else
		light = 0;
	return (light);
}


void	calc_normal(t_vector point, t_vector direction, t_vector *normal, t_figure *figure)
{
	if (figure->type == SPHERE)
	{
		*normal = vector_norm(vector_sub(point, figure->figure.sp.center));
		if (vector_cos(direction, *normal) > 0)
		{
			*normal = vector_mlt(-1, *normal);
			figure->figure.sp.inside = 1;
		}
		else
			figure->figure.sp.inside = 0;
	}
	else if (vector_cos(direction, figure->normal) > 0)
		*normal = vector_mlt(-1, figure->normal);
	else
		*normal = figure->normal;
}


int			trace_ray(t_vector_2p ray, int depth, t_figure *lst, t_scene *scene)
{
	t_figure	closest_figure;
	t_inter		*inter;
	double		closest_intersection;
	double		r;
	t_vector_2p	new_ray;
	int			ret_color;

	inter = (t_inter *)memalloc(sizeof(t_inter));
	closest_intersection = INFINITY;
	closest_figure.type = -1;
	get_closest_inter(ray, lst, &closest_figure, &closest_intersection);
	inter->point = vector_sum(ray.origin, vector_mlt(closest_intersection, ray.direction));
	calc_normal(inter->point, ray.direction, &inter->normal, &closest_figure);
	inter->color = scene->background;
	if (closest_figure.type != -1)
		inter->color = closest_figure.color;
	else
		return (scene->background);
	inter->old_normal = inter->normal;
	apply_texture(&closest_figure, inter, scene);
	inter->figure = &closest_figure;
	calc_light(ray, inter, *scene, lst);
	r = 0;
	if (closest_figure.type != -1)
		r = closest_figure.reflection;
	inter->reflection_color = 0;
	if (r > 0 && depth > 0)
	{
		new_ray.origin = inter->point;
		new_ray.direction = reflect_ray(vector_mlt(-1, ray.direction), inter->normal);
		if (vector_dot(vector_mlt(1, new_ray.direction), inter->old_normal) >= 0)
			inter->reflection_color = trace_ray(new_ray, depth - 1, lst, scene);
	}
	ret_color = color_sum(color_mlt(inter->color, 1 - r), color_mlt(inter->reflection_color, r));
	free(inter);
	return (ret_color);
}

int main(int argc, const char **argv)
{
	t_mlx		mlx;
	t_scene		scene;
	t_figure	*figure;
	t_camera	*curr;

	if (argc != 2)
		ft_usage_message(argv[0]);
	parse_scene(&mlx, &scene, &figure, argv);
	init_mlx(&mlx, &scene);
	curr = mlx.camera;
	load_map(&scene);
	scene.background = 0;
	while (curr)
	{
		render_scene(&scene, figure, &mlx, curr);
		curr = curr->next;
	}
	mlx.window = mlx_new_window(mlx.mlx, scene.width, scene.height, "miniRT");
	mlx_put_image_to_window(mlx.mlx, mlx.window , mlx.camera->image, 0, 0);
	mlx_hook(mlx.window, 2, 1L<<0, key_handler, &mlx);
	mlx_hook(mlx.window, 17, 0L, red_cross_handler, &mlx);
	mlx_loop(mlx.mlx);
	return (0);
}
