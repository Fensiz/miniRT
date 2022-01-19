/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 17:30:01 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/19 17:30:02 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	solve_plane(t_vector origin, t_vector direction, t_vector plane_p,
	t_vector plane_nv)
{
	double	x;
	double	denom;

	denom = vector_dot(plane_nv, direction);
	if (denom == 0)
		return (INFINITY);
	x = (vector_dot(plane_nv, vector_sub(plane_p, origin))) / denom;
	if (x > 0)
		return (x);
	else
		return (INFINITY);
}

double	plane_intersection(t_vector origin, t_vector d, t_figure *lst)
{
	return (solve_plane(origin, d, lst->figure.pl.point, lst->normal));
}
