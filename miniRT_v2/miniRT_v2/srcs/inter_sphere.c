/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 17:26:55 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/19 17:26:57 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	solve_square_exp(double k[3], double x[2])
{
	double	discr;

	discr = pow(k[1], 2) - 4 * k[0] * k[2];
	if (discr < 0)
	{
		x[0] = INFINITY;
		x[1] = INFINITY;
		return (0);
	}
	x[0] = (-k[1] + sqrt(discr)) / (2 * k[0]);
	x[1] = (-k[1] - sqrt(discr)) / (2 * k[0]);
	return (1);
}

static void	solve_sphere(double x[2], t_vector_2p ray,
	t_figure *figure)
{
	t_vector	cam_sphere;
	double		k[3];

	cam_sphere = vector_sub(ray.origin, figure->figure.sp.center);
	k[0] = vector_dot(ray.direction, ray.direction);
	k[1] = 2 * vector_dot(ray.direction, cam_sphere);
	k[2] = vector_dot(cam_sphere, cam_sphere) - figure->figure.sp.radius
		* figure->figure.sp.radius;
	solve_square_exp(k, x);
}

double	sphere_intersection(t_vector_2p ray, t_figure *figure)
{
	double		closest;
	double		x[2];

	closest = INFINITY;
	solve_sphere(x, ray, figure);
	if (x[0] > EPSILON && x[0] < INFINITY)
		closest = x[0];
	if (x[1] > EPSILON && x[1] < INFINITY && x[1] < x[0])
		closest = x[1];
	return (closest);
}
