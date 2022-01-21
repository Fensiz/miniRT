/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 15:35:35 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 15:35:36 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_uv	uv_sphere(t_inter *inter, t_figure *figure)
{
	t_vector	coords;
	double		phi;
	double		theta;
	t_uv		i;

	coords = vector_sub(inter->point, figure->figure.sp.center);
	theta = atan2(coords.x, coords.z);
	phi = acos((double)coords.y / figure->figure.sp.radius);
	i.u = 1 - (theta / (2 * M_PI) + 0.5);
	i.v = 1 - phi / M_PI;
	return (i);
}

static void	texture_sphere(t_inter *inter, t_figure *figure, t_map *map)
{
	t_uv		i;

	i = uv_sphere(inter, figure);
	if (figure->texture >> 1)
		bump_texture(i, figure, inter, map);
	if (figure->texture & 1)
		checkerboard_texture(i, inter, figure->type);
}

static void	cylinder_case(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;

	coords = vector_sub(inter->point, figure->figure.cy.center);
	coords.x += EPSILON;
	coords.y += EPSILON;
	coords.z += EPSILON;
	coords = rot_from_n_to_y1(coords, figure->figure.cy.nv);
	if (fabs(coords.y) < 0.01
		|| fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
		texture_plane(figure, inter, map);
	else
		texture_cylinder(figure, inter, map);
}

static void	cone_case(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;

	coords = vector_sub(inter->point, figure->figure.co.center);
	coords.x += EPSILON;
	coords.y += EPSILON;
	coords.z += EPSILON;
	coords = rot_from_n_to_y1(coords, figure->figure.co.nv);
	if (fabs(coords.y) < 0.01
		|| fabs(fabs(coords.y) - figure->figure.co.height) < 0.01)
		texture_plane(figure, inter, map);
	else
		texture_cone(figure, inter, map);
}

void	apply_texture(t_figure *figure, t_inter *inter, t_scene *scene)
{
	if (figure->type == CYLINDER)
		cylinder_case(figure, inter, scene->map);
	else if (figure->type == CONE)
		cone_case(figure, inter, scene->map);
	else if (figure->type == SPHERE)
		texture_sphere(inter, figure, scene->map);
	else
		texture_plane(figure, inter, scene->map);
}
