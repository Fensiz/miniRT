/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 23:57:35 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/19 23:57:39 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	solve_cylinder(double x[2], t_vector_2p ray, t_figure *lst)
{
	t_vector	v;
	t_vector	u;
	double		k[3];

	v = vector_mlt(vector_dot(ray.direction, lst->figure.cy.nv),
			lst->figure.cy.nv);
	v = vector_sub(ray.direction, v);
	u = vector_mlt(vector_dot(vector_sub(ray.origin, lst->figure.cy.center),
				lst->figure.cy.nv), lst->figure.cy.nv);
	u = vector_sub(vector_sub(ray.origin, lst->figure.cy.center), u);
	k[0] = vector_dot(v, v);
	k[1] = 2 * vector_dot(v, u);
	k[2] = vector_dot(u, u) - pow(lst->figure.cy.radius, 2);
	return (solve_square_exp(k, x));
}

static	int	check_point(double x2[2], int i, t_figure *lst)
{
	if (lst->figure.cy.dist[i] >= 0
		&& lst->figure.cy.dist[i] <= lst->figure.cy.height
		&& x2[i] > EPSILON)
		return (1);
	return (0);
}

static	void	cy_inter_x(double x2[2], double *dist, double *x, t_figure *lst)
{
	if (check_point(x2, 0, lst) && check_point(x2, 1, lst))
	{
		*dist = lst->figure.cy.dist[1];
		*x = x2[1];
		if (x2[0] < x2[1])
		{
			*dist = lst->figure.cy.dist[0];
			*x = x2[0];
		}
	}
	else if (check_point(x2, 0, lst))
	{
		*dist = lst->figure.cy.dist[0];
		*x = x2[0];
	}
	else
	{
		*dist = lst->figure.cy.dist[1];
		*x = x2[1];
	}
}

static t_vector	calc_cy_normal(double x2[2], t_vector_2p ray, t_figure *lst)
{
	double		dist;
	double		x;
	t_vector	v;
	t_vector	c;

	cy_inter_x(x2, &dist, &x, lst);
	x2[0] = x;
	v = vector_sub(vector_mlt(x, ray.direction),
			vector_mlt(dist, lst->figure.cy.nv));
	c = vector_sub(lst->figure.cy.center, ray.origin);
	return (vector_norm(vector_sub(v, c)));
}

double	cy_intersection(t_vector_2p ray, t_vector *normal, t_figure *lst)
{
	double	x2[2];

	if (solve_cylinder(x2, ray, lst) == 0)
		return (INFINITY);
	lst->figure.cy.dist[0] = vector_dot(lst->figure.cy.nv,
			vector_sub(vector_mlt(x2[0], ray.direction),
				vector_sub(lst->figure.cy.center, ray.origin)));
	lst->figure.cy.dist[1] = vector_dot(lst->figure.cy.nv,
			vector_sub(vector_mlt(x2[1], ray.direction),
				vector_sub(lst->figure.cy.center, ray.origin)));
	if (!(check_point(x2, 0, lst) || check_point(x2, 1, lst)))
		return (INFINITY);
	*normal = calc_cy_normal(x2, ray, lst);
	return (x2[0]);
}
