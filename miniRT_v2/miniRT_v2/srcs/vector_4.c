/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:26:27 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:26:29 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

static	double	rot_angle(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + coord * coord)) / M_PI) * 180);
}

static	double	rot_angle2(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + n.x * n.x + n.z * n.z))
			/ M_PI) * 180);
}

t_vector	rot_from_n_to_y1(t_vector v, t_vector n)
{
	if (n.y >= 0)
	{
		v = vector_x_rot(v, -rot_angle(n, n.z));
		n = vector_x_rot(n, -rot_angle(n, n.z));
	}
	else
	{
		v = vector_x_rot(v, 180 + rot_angle(n, n.z));
		n = vector_x_rot(n, 180 + rot_angle(n, n.z));
	}
	if (n.y >= 0)
		v = vector_z_rot(v, rot_angle(n, n.x));
	else
		v = vector_z_rot(v, 180 - rot_angle(n, n.x));
	return (v);
}

t_vector	rot_from_y1_to_n(t_vector v, t_vector n)
{
	if (n.x >= 0 && n.y >= 0)
	{
		v = vector_z_rot(v, -rot_angle2(n, n.x));
		v = vector_x_rot(v, rot_angle(n, n.z));
	}
	else if (n.x < 0 && n.y >= 0)
	{
		v = vector_z_rot(v, -rot_angle2(n, n.x));
		v = vector_x_rot(v, rot_angle(n, n.z));
	}
	else if (n.x >= 0 && n.y < 0)
	{
		v = vector_z_rot(v, -rot_angle2(n, n.x));
		v = vector_x_rot(v, 180 - rot_angle(n, n.z));
	}
	else if (n.x < 0 && n.y < 0)
	{
		v = vector_z_rot(v, -rot_angle2(n, n.x));
		v = vector_x_rot(v, 180 - rot_angle(n, n.z));
	}
	return (v);
}
