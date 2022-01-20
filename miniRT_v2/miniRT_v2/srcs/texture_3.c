/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 15:35:40 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 15:35:42 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	texture_plane(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;
	t_uv		i;

	coords = rot_from_n_to_y1(inter->point, inter->normal);
	i.u = coords.x;
	i.v = coords.z;
	if (figure->texture >> 1)
		bump_texture(i, figure, inter, map);
	if (figure->texture & 1)
	{
		coords.x = abs((int)floor((coords.x * 2) + EPSILON)) % 2;
		coords.z = abs((int)floor((coords.z * 2) + EPSILON)) % 2;
		if (((int)coords.x) ^ (int)coords.z)
			inter->color = BLACK;
		else
			inter->color = WHITE;
	}
}

void	texture_cylinder(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;
	t_uv		i;
	double		theta;

	coords = vector_sub(inter->point, figure->figure.cy.center);
	coords.x += EPSILON;
	coords.y += EPSILON;
	coords.z += EPSILON;
	coords = rot_from_n_to_y1(coords, figure->figure.cy.nv);
	theta = atan2(coords.x, coords.z);
	i.u = -theta / M_PI * figure->figure.cy.radius * 4;
	i.v = coords.y;
	if (figure->texture >> 1)
		bump_texture(i, figure, inter, map);
	if (figure->texture & 1)
		checkerboard_texture(i, inter, figure->type);
}

void	texture_cone(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;
	t_uv		i;
	double		theta;

	coords = vector_sub(inter->point, figure->figure.co.center);
	coords.x += EPSILON;
	coords.y += EPSILON;
	coords.z += EPSILON;
	coords = rot_from_n_to_y1(coords, figure->figure.co.nv);
	theta = atan2(coords.x, coords.z);
	i.u = theta / M_PI * figure->figure.co.radius * 4;
	i.v = coords.y;
	if (figure->texture >> 1)
		bump_texture(i, figure, inter, map);
	if (figure->texture & 1)
		checkerboard_texture(i, inter, figure->type);
}
