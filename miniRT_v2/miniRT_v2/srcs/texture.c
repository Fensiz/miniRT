/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 15:35:31 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 15:35:32 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	uv_to_normal(double u, double v, int *map, int map_size)
{
	int			ui;
	int			vi;
	t_vector	g;

	u *= 20;
	v *= 20;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	g.x = (map[(ui - 1 + map_size) % map_size + vi * map_size]
			- map[(ui + 1) % map_size + vi * map_size]);
	g.y = EPSILON;
	g.z = -(map[ui + (vi - 1 + map_size) % map_size * map_size]
			- map[ui + (vi + 1) % map_size * map_size]);
	if (g.x == 0 && g.z == 0)
		return (vector_set(0, 0, 0));
	g = vector_norm(g);
	return (g);
}

void	checkerboard_texture(t_uv i, t_inter *inter, int type)
{
	t_vector	val;

	val.x = abs((int)floor(i.u)) % 2;
	val.y = abs((int)floor(i.v)) % 2;
	if (type == SPHERE)
	{
		val.x *= 40;
		val.y *= 20;
	}
	if (((int)val.x) ^ (int)val.y)
		inter->color = BLACK;
	else
		inter->color = WHITE;
}

void	bump_texture(t_uv i, int type, t_inter *inter, t_map *map)
{
	t_vector	gx;

	if (type == SPHERE)
		gx = uv_to_normal(40 * i.u, 20 * i.v, map->map, map->size);
	else
		gx = uv_to_normal(i.u, i.v, map->map, map->size);
	if (vector_len(gx))
	{
		gx = vector_mlt(0.2, gx);
		gx = vector_sum(gx, vector_set(0, 1, 0));
		gx = vector_norm(gx);
		inter->normal = rot_from_y1_to_n(gx, inter->normal);
	}
}
