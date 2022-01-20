/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 00:06:41 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 00:06:43 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	reflect_ray(t_vector ray, t_vector normal)
{
	return (vector_sub(vector_mlt(2 * vector_dot(normal, ray), normal), ray));
}

static	double	calc_specular(t_vector_2p ray, t_inter *inter,
	t_scene scene, t_figure *lst)
{
	double		light;
	t_vector	direction;
	t_vector	to_cam;
	t_vector	reflected;

	direction = vector_sub(scene.light->origin, inter->point);
	to_cam = vector_sub(ray.origin, inter->point);
	reflected = reflect_ray(direction, inter->normal);
	if (vector_dot(reflected, to_cam) > 0)
		light = scene.light->brightness
			* pow(vector_cos(reflected, to_cam), lst->specular);
	else
		light = 0;
	return (light);
}

static	int	in_light(t_vector o, t_vector i, t_vector d, t_figure *lst)
{
	double		distance_v;
	double		distance_mem;
	t_vector_2p	ray;

	ray = vector_2p_set(o, d);
	distance_v = INFINITY;
	distance_mem = INFINITY;
	while (lst)
	{
		if (lst->type == SPHERE)
			distance_v = sphere_intersection(ray, lst);
		else if (lst->type == PLANE)
			distance_v = plane_intersection(ray, lst);
		else if (lst->type == CYLINDER)
			distance_v = cylinder_intersection(ray, lst);
		else if (lst->type == CONE)
			distance_v = cone_intersection(ray, lst);
		if (distance_v < distance_mem)
			distance_mem = distance_v;
		lst = lst->next;
	}
	if (distance_mem < INFINITY && vector_len(vector_sub(i,
				vector_sum(vector_mlt(distance_mem, d), o))) < 0.01)
		return (1);
	return (0);
}

void	calc_light(t_vector_2p ray, t_inter *inter,
	t_scene scene, t_figure *lst)
{
	double			light;
	double			rgb[3];
	t_vector		direction;

	ft_memset(rgb, 0, 3 * sizeof(double));
	add_coeficient(rgb, scene.ambient_light, scene.ambient_light_color);
	while (scene.light)
	{
		direction = vector_norm(vector_sub(scene.light->origin, inter->point));
		if (in_light(scene.light->origin, inter->point,
				vector_mlt(-1, direction), lst)
			&& vector_dot(inter->normal, direction) > 0)
		{
			light = scene.light->brightness
				* vector_cos(inter->normal, direction);
			add_coeficient(rgb, light, scene.light->color);
		}
		if (inter->figure->specular)
		{
			light = calc_specular(ray, inter, scene, inter->figure);
			add_coeficient(rgb, light, scene.light->color);
		}
		scene.light = scene.light->next;
	}
	inter->color = apply_light(inter->color, rgb);
}
