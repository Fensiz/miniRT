#include "minirt.h"
#include <mlx.h>
#include <math.h>

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
//задать начальное значение ambient light
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
//
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
#include <stdio.h>
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
}

static void	parse_elems(t_mlx *mlx, t_scene *scene, t_figure **figure, char *str)
{
	while (*str)
	{
		if (*str == '#')
		{
			while (*str && *str != '\n')
				str++;
		}
		else
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

void	parse_scene(t_mlx *mlx, t_scene *scene, t_figure **figure,
							const char **argv)
{
	char		*str;
	int			fd;

	*figure = NULL;
	scene->light = NULL;
	mlx->camera = NULL;
	fd = open(argv[1], O_RDONLY);
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

#include <stdio.h>

static t_vector		cam_rot(t_vector d, t_vector cam_direction)
{
	t_vector	x_axis;
	t_vector	y_axis;
	t_vector	z_axis;
	t_vector	rotated;

	z_axis = cam_direction;
	if (cam_direction.y == 1)
		x_axis = vector_set(1, 0, 0);
	else if (cam_direction.y == -1)
		x_axis = vector_set(-1, 0, 0);
	else
		x_axis = vector_cross(vector_set(0, 1, 0), z_axis);
	y_axis = vector_cross(z_axis, x_axis);
	rotated.x = d.x * x_axis.x + d.y * y_axis.x + d.z * z_axis.x;
	rotated.y = d.x * x_axis.y + d.y * y_axis.y + d.z * z_axis.y;
	rotated.z = d.x * x_axis.z + d.y * y_axis.z + d.z * z_axis.z;
	return (rotated);
}

void		render_scene(t_scene *scene, t_figure *figure, t_mlx *mlx, t_camera *camera)
{
	int			color;
	int			x;
	int			y;
	t_vector_2p	ray;

	y = 0;
	ray.origin = camera->origin;
	while (y < scene->height)
	{
		x = 0;
		while (x < scene->width)
		{
			ray.direction = canvas_to_viewport(x, y, camera, scene);
			ray.direction = cam_rot(ray.direction, camera->direction);
			color = trace_ray(ray, REFLECTION_LIMIT, figure, scene);
			my_put_pixel(mlx, x, y, color);
			x++;
		}
		y++;
	}
}

void		try_all_intersections(t_vector_2p ray, t_figure *figure,
					t_figure *closest_figure, double *closest_intersection)
{
	double distance;

	distance = 0;
	while (figure)
	{
		if (figure->type == SPHERE)
			distance = sphere_intersection(ray.origin, ray.direction, figure);
		else if (figure->type == PLANE)
			distance = plane_intersection(ray.origin, ray.direction, figure);
		else if (figure->type == CYLINDER)
			distance = cylinder_intersection(ray.origin, ray.direction, figure);
		else if (figure->type == CONE)
		{
			//printf("+\n");
			distance = cone_intersection(ray.origin, ray.direction, figure);
		}
		if (distance > EPSILON && distance < *closest_intersection)
		{
			*closest_figure = *figure;
			*closest_intersection = distance;
		}
		figure = figure->next;
	}
}
//o - light origin
//i - inter point
int		in_light2(t_vector o, t_vector i, t_vector d, t_figure *lst) //shadows
{
	double	distance_v = INFINITY;
	double	distance_mem = INFINITY;
	t_figure	closest_figure;

	while (lst)
	{
		if (lst->type == SPHERE)
			distance_v = sphere_intersection(o, d, lst);
		else if (lst->type == PLANE)
			distance_v = plane_intersection(o, d, lst);
		else if (lst->type == CYLINDER)
			distance_v = cylinder_intersection(o, d, lst);
		else if (lst->type == CONE)
			distance_v = cone_intersection(o, d, lst);
		if (distance_v < distance_mem)
		{
			distance_mem = distance_v;
			closest_figure = *lst;
		}
		
//		if (distance > EPSILON && distance < 1 && lst->type != CONE)
//			return (0);
		lst = lst->next;
	}
	if (distance_mem < INFINITY && vector_len(vector_sub(i, vector_sum(vector_mlt(distance_mem, d), o))) < 0.1)
	{
		return 1;
	}
	return (0);
}
int		in_light(t_vector o, t_vector d, t_figure *lst) //shadows
{
	double	distance;

	while (lst)
	{
		if (lst->type == SPHERE)
			distance = sphere_intersection(o, d, lst);
		else if (lst->type == PLANE)
			distance = plane_intersection(o, d, lst);
		else if (lst->type == CYLINDER)
			distance = cylinder_intersection(o, d, lst);
		else if (lst->type == CONE)
			distance = cone_intersection(o, d, lst);
		if (distance > EPSILON && distance < 1 && lst->type != CONE)
			return (0);
		lst = lst->next;
	}
	return (1);
}
void	add_coeficient(double *rgb, double coef, int color)
{
	t_color	color_c;

	color_c = convert_color2c(color);
	rgb[0] += coef * color_c.red / 255;
	rgb[1] += coef * color_c.green / 255;
	rgb[2] += coef * color_c.blue / 255;
}
t_vector	reflect_ray(t_vector ray, t_vector normal)
{
	return (vector_sub(vector_mlt(2 * vector_dot(normal, ray), normal), ray));
}
double	calc_specular(t_vector_2p ray, t_inter *inter, t_scene scene, t_figure *lst)
{
	double	light;
	t_vector	direction;
	t_vector	p_to_cam;
	t_vector	reflected;

	direction = vector_sub(scene.light->origin, inter->point);
	p_to_cam = vector_sub(ray.origin, inter->point);
	reflected = reflect_ray(direction, inter->normal);
	if (vector_dot(reflected, p_to_cam) > 0)
		light = scene.light->brightness * pow(vcos(reflected, p_to_cam), lst->specular);
	else
		light = 0;
	return (light);
}
void	compute_light(t_vector_2p ray, t_inter *inter, t_scene scene, t_figure *lst)
{
	double			light;
	double			rgb[3];
	t_vector		direction;
	t_vector		direction2;

	ft_memset(rgb, 0, 3 * sizeof(double));
	add_coeficient(rgb, scene.ambient_light, scene.ambient_light_color);
	while (scene.light)
	{
		direction = vector_sub(scene.light->origin, inter->point);
		direction2 = vector_norm(vector_sub(inter->point, scene.light->origin));
		if (//in_light(inter->point, direction, lst)
			in_light2(scene.light->origin ,inter->point, direction2, lst)
			&&
			vector_dot(inter->normal, direction) > 0)
		{
			light = scene.light->brightness * vcos(inter->normal, direction);
			add_coeficient(rgb, light, scene.light->color);
		}
		if (lst->specular)
		{
			light = calc_specular(ray, inter, scene, lst);
			add_coeficient(rgb, light, scene.light->color);
		}
		scene.light = scene.light->next;
	}
	inter->color = color_x_light_new(inter->color, rgb);
}

void	calc_normal(t_vector point, t_vector direction, t_vector *normal, t_figure *light)
{
	if (light->type == SPHERE)
	{
		*normal = vector_norm(vector_sub(point, light->figure.sp.center));
		if (vcos(direction, *normal) > 0)
		{
			*normal = vector_mlt(-1, *normal);
			light->figure.sp.inside = 1;
		}
		else
			light->figure.sp.inside = 0;
	}
	else if (vcos(direction, light->normal) > 0)
		*normal = vector_mlt(-1, light->normal);
	else
		*normal = light->normal;
}


int			trace_ray(t_vector_2p ray, int depth, t_figure *lst, t_scene *scene)
{
	t_figure	closest_figure;
	t_inter		inter;
	double		closest_intersection;
	double		r;
	t_vector_2p	new_ray;

	closest_intersection = INFINITY;
	closest_figure.type = -1;
	try_all_intersections(ray, lst, &closest_figure, &closest_intersection);
	inter.point = vector_sum(ray.origin, vector_mlt(closest_intersection, ray.direction));
	calc_normal(inter.point, ray.direction, &inter.normal, &closest_figure);
	inter.color = scene->background;
	if (closest_figure.type != -1)
		inter.color = closest_figure.color;
	//return (inter.color);
	apply_texture(&closest_figure, &inter);
	compute_light(ray, &inter, *scene, lst);
	

	r = 0;
	if (closest_figure.type != -1)
		r = closest_figure.reflection;
	if (r > 0 && depth > 0)
	{
		new_ray.origin = inter.point;
		new_ray.direction = reflect_ray(vector_mlt(-1, ray.direction), inter.normal);
		inter.reflection_color = trace_ray(new_ray, depth - 1, lst, scene);
	}
	return (color_sum(color_mlt(inter.color, 1 - r), color_mlt(inter.reflection_color, r)));
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
	while (curr)
	{
		render_scene(&scene, figure, &mlx, curr);
		curr = curr->next;
	}
//	success_message(ac);
	
//	graphic_loop(mlx, scene);
	mlx.window = mlx_new_window(mlx.mlx, scene.width, scene.height, "miniRT");
	mlx_put_image_to_window(mlx.mlx, mlx.window , mlx.camera->image, 0, 0);
	mlx_loop(mlx.mlx);
	return (0);
}
