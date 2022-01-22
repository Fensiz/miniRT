/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 23:07:41 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 23:07:43 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	if (cam_direction.z < 0)
	{
		cam_direction.y = -cam_direction.y;
	}
	rotated = rot_from_y1_to_n(d, cam_direction);
	if (cam_direction.z < 0)
		rotated = vector_z_rot(rotated, 180);
	return (rotated);
}

void	render_scene(t_scene *scene, t_figure *figure, t_mlx *mlx,
			t_camera *camera)
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
	double	distance;

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

void	calc_normal(t_vector point, t_vector direction, t_vector *normal,
			t_figure *figure)
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
