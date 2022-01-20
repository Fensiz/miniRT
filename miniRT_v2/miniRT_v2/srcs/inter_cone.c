/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 11:49:36 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 11:49:38 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static double	cap_intersection(t_vector o, t_vector d, t_figure *lst)
{
	double		id1;
	t_vector	ip1;
	t_vector_2p	ray;

	ray = vector_2p_set(o, d);
	id1 = solve_plane(ray, lst->figure.co.center, lst->figure.co.nv);
	if (id1 < INFINITY)
	{
		ip1 = vector_sum(o, vector_mlt(id1, d));
		if (id1 < INFINITY
			&& distance(ip1, lst->figure.co.center) <= lst->figure.co.radius)
			return (id1);
		return (INFINITY);
	}
	return (INFINITY);
}

static int	cone_solve(t_vector_2p ray, double x[2], t_figure *lst)
{
	double		k[3];
	t_vector	center;
	t_vector	co;
	t_vector	v;
	double		theta;

	center = vector_sum(lst->figure.co.center, vector_mlt(lst->figure.co.height,
				lst->figure.co.nv));
	co = vector_sub(ray.origin, center);
	v = vector_mlt(-1, lst->figure.co.nv);
	theta = atan(lst->figure.co.radius / lst->figure.co.height);
	k[0] = pow(vector_dot(ray.direction, v), 2) - pow(cos(theta), 2);
	k[1] = 2 * (vector_dot(ray.direction, v) * vector_dot(co, v)
			- vector_dot(ray.direction, co) * pow(cos(theta), 2));
	k[2] = pow(vector_dot(co, v), 2) - vector_dot(co, co) * pow(cos(theta), 2);
	return (solve_square_exp(k, x));
}

int	calc_co_normal(t_vector_2p ray, double x, t_vector *normal, t_figure *lst)
{
	t_vector	cp;
	double		h;
	t_vector	v;
	t_vector	center;

	center = vector_sum(lst->figure.co.center, vector_mlt(lst->figure.co.height,
				lst->figure.co.nv));
	v = vector_mlt(-1, lst->figure.co.nv);
	cp = vector_sub(vector_sum(ray.origin,
				vector_mlt(x, ray.direction)), center);
	h = vector_dot(cp, v);
	if (h < EPSILON || h > lst->figure.co.height)
		return (0);
	*normal = vector_norm(vector_sub(vector_mlt(vector_dot(v, cp)
					/ vector_dot(cp, cp), cp), v));
	return (1);
}

static double	co_intersection(t_vector_2p ray, t_vector *normal,
					t_figure *lst)
{
	double	x2[2];
	double	x;

	if (!cone_solve(ray, x2, lst))
		return (INFINITY);
	x = x2[0];
	if (x < EPSILON || (x2[1] > EPSILON && x2[1] < x))
		x = x2[1];
	if (x < EPSILON)
		return (INFINITY);
	if (!calc_co_normal(ray, x, normal, lst))
		return (INFINITY);
	return (x);
}

double	cone_intersection(t_vector_2p ray, t_figure *lst)
{
	double		cone_inter;
	double		cap_inter;
	t_vector	co_normal;

	cone_inter = co_intersection(ray, &co_normal, lst);
	cap_inter = cap_intersection(ray.origin, ray.direction, lst);
	if (cone_inter < INFINITY || cap_inter < INFINITY)
	{
		if (cone_inter < cap_inter)
		{
			lst->normal = co_normal;
			return (cone_inter);
		}
		lst->normal = lst->figure.co.nv;
		return (cap_inter);
	}
	return (INFINITY);
}
