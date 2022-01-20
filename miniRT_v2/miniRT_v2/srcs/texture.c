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

	if (type == SPHERE)
	{
		i.u *= 20;
		i.v *= 10;
	}
	i.u *= 2;
	i.v *= 2;
	val.x = abs((int)floor(i.u)) % 2;
	val.y = abs((int)floor(i.v)) % 2;
	if (((int)val.x) ^ (int)val.y)
		inter->color = BLACK;
	else
		inter->color = WHITE;
}

t_map	*map_select(t_figure *figure, t_map *map_list)
{
	t_map	*map;
	int		bump;

	bump = figure->texture >> 1;
	if (bump == 1)
		map = map_list;
	else if (bump == 2)
		map = map_list->next;
	else
		map = map_list->next->next;
	return (map);
}

void	bump_texture(t_uv i, t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	gx;
	t_map		*smap;

	smap = map_select(figure, map);
	if (figure->type == SPHERE)
		gx = uv_to_normal(40 * i.u, 20 * i.v, smap->map, smap->size);
	else
		gx = uv_to_normal(2 * i.u, 2 * i.v, smap->map, smap->size);
	if (vector_len(gx))
	{
		gx = vector_mlt(0.2, gx);
		gx = vector_sum(gx, vector_set(0, 1, 0));
		gx = vector_norm(gx);
		inter->normal = rot_from_y1_to_n(gx, inter->normal);
	}
}
