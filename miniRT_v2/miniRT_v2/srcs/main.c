#include "minirt.h"
#include <mlx.h>
#include <math.h>
#include <stdio.h>

void	ft_usage_message(const char *program_name)
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

static t_vector	canvas_to_viewport(double x, double y, t_camera *camera,
	t_scene *scene)
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

static t_vector	cam_rot(t_vector d, t_vector cam_direction)
{
	t_vector	rotated;

	d = vector_x_rot(d, -90);
	rotated = rot_from_y1_to_n(d, cam_direction);
	return (rotated);
}

void	render_scene(t_scene *scene, t_figure *figure, t_mlx *mlx, t_camera *camera)
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
			*normal = vector_mlt(-1, *normal);
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
	ret_color = color_sum(color_mlt(inter->color, 1 - r),
			color_mlt(inter->reflection_color, r));
	free(inter);
	return (ret_color);
}

int	main(int argc, const char **argv)
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
	mlx_put_image_to_window(mlx.mlx, mlx.window, mlx.camera->image, 0, 0);
	mlx_hook(mlx.window, 2, 1L << 0, key_handler, &mlx);
	mlx_hook(mlx.window, 17, 0L, red_cross_handler, &mlx);
	mlx_loop(mlx.mlx);
	return (0);
}
