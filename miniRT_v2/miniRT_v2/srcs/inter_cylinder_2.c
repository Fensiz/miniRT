/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_cylinder_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 00:01:42 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 00:01:45 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static	int	check_dist(t_interv v, int i, t_vector center, t_figure *lst)
{
	if (v.id[i] < INFINITY
		&& distance(v.ip[i], center) <= lst->figure.cy.radius)
		return (1);
	return (0);
}

static double	caps_intersection(t_vector_2p ray, t_figure *lst)
{
	t_interv	i;
	t_vector	c2;

	c2 = vector_sum(lst->figure.cy.center, vector_mlt(lst->figure.cy.height,
				lst->figure.cy.nv));
	i.id[0] = solve_plane(ray, lst->figure.cy.center, lst->figure.cy.nv);
	i.id[1] = solve_plane(ray, c2, lst->figure.cy.nv);
	if (i.id[0] < INFINITY || i.id[1] < INFINITY)
	{
		i.ip[0] = vector_sum(ray.origin, vector_mlt(i.id[0], ray.direction));
		i.ip[1] = vector_sum(ray.origin, vector_mlt(i.id[1], ray.direction));
		if (check_dist(i, 0, lst->figure.cy.center, lst)
			&& check_dist(i, 1, c2, lst))
		{
			if (i.id[0] < i.id[1])
				return (i.id[0]);
			return (i.id[1]);
		}
		else if (check_dist(i, 0, lst->figure.cy.center, lst))
			return (i.id[0]);
		else if (check_dist(i, 1, c2, lst))
			return (i.id[1]);
		return (INFINITY);
	}
	return (INFINITY);
}

double	cylinder_intersection(t_vector_2p ray, t_figure *lst)
{
	double		cylinder_inter;
	double		caps_inter;
	t_vector	cy_normal;

	cylinder_inter = cy_intersection(ray, &cy_normal, lst);
	caps_inter = caps_intersection(ray, lst);
	if (cylinder_inter < INFINITY || caps_inter < INFINITY)
	{
		if (cylinder_inter < caps_inter)
		{
			lst->normal = cy_normal;
			return (cylinder_inter);
		}
		lst->normal = lst->figure.cy.nv;
		return (caps_inter);
	}
	return (INFINITY);
}
