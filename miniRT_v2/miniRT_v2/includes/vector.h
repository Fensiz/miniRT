/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:06:48 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 13:06:49 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H
# include <math.h>

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;
typedef struct s_vector_2p
{
	t_vector	origin;
	t_vector	direction;
}	t_vector_2p;
t_vector	vector_set(double x, double y, double z);
t_vector	vector_sum(t_vector a, t_vector b);
t_vector	vector_sub(t_vector a, t_vector b);
double		vector_dot(t_vector a, t_vector b);
t_vector	vector_cross(t_vector a, t_vector b);
double		vector_len(t_vector v);
t_vector	vector_norm(t_vector v);
double		vector_cos(t_vector a, t_vector b);
double		vector_sin(t_vector a, t_vector b);
t_vector	vector_mlt(double n, t_vector v);
double		distance(t_vector p1, t_vector p2);
t_vector	vector_x_rot(t_vector v, double degree_angle);
t_vector	vector_y_rot(t_vector v, double degree_angle);
t_vector	vector_z_rot(t_vector v, double degree_angle);
t_vector_2p	vector_2p_set(t_vector origin, t_vector direction);
t_vector	rot_from_n_to_y1(t_vector v, t_vector n);
t_vector	rot_from_y1_to_n(t_vector v, t_vector n);
#endif
