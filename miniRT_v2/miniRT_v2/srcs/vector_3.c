/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 00:12:54 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 00:12:55 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

double	distance(t_vector p1, t_vector p2)
{
	double	ret;

	ret = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
	return (ret);
}

t_vector	vector_x_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(1, 0, 0);
	rot_mtx[1] = vector_set(0, cos(rad_angle), -sin(rad_angle));
	rot_mtx[2] = vector_set(0, sin(rad_angle), cos(rad_angle));
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector	vector_y_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(cos(rad_angle), 0, sin(rad_angle));
	rot_mtx[1] = vector_set(0, 1, 0);
	rot_mtx[2] = vector_set(-sin(rad_angle), 0, cos(rad_angle));
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector	vector_z_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(cos(rad_angle), -sin(rad_angle), 0);
	rot_mtx[1] = vector_set(sin(rad_angle), cos(rad_angle), 0);
	rot_mtx[2] = vector_set(0, 0, 1);
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector_2p	vector_2p_set(t_vector origin, t_vector direction)
{
	t_vector_2p	ray;

	ray.origin = origin;
	ray.direction = direction;
	return (ray);
}
