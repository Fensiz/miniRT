/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 00:12:48 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 00:12:50 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

double	vector_len(t_vector v)
{
	return (sqrt(fabs(vector_dot(v, v))));
}

t_vector	vector_norm(t_vector v)
{
	t_vector	ret;
	double		len;

	len = vector_len(v);
	ret.x = v.x / len;
	ret.y = v.y / len;
	ret.z = v.z / len;
	return (ret);
}

double	vector_cos(t_vector a, t_vector b)
{
	return (vector_dot(a, b) / (vector_len(a) * vector_len(b)));
}

double	vector_sin(t_vector a, t_vector b)
{
	return (sqrt(1 - pow(vector_cos(a, b), 2)));
}

t_vector	vector_mlt(double n, t_vector v)
{
	t_vector	ret;

	ret.x = n * v.x;
	ret.y = n * v.y;
	ret.z = n * v.z;
	return (ret);
}
