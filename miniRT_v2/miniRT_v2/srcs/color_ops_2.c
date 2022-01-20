/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_ops_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 12:18:44 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 12:18:48 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "color.h"

int	color_sum(int i_color_1, int i_color_2)
{
	t_color	color_1;
	t_color	color_2;

	color_1 = convert_color2c(i_color_1);
	color_2 = convert_color2c(i_color_2);
	color_1.red += color_2.red;
	color_1.green += color_2.green;
	color_1.blue += color_2.blue;
	return (convert_color2i(color_1));
}

static	void	fix_color(t_color	*color)
{
	if (color->red > 255)
		color->red = 255;
	if (color->green > 255)
		color->green = 255;
	if (color->blue > 255)
		color->blue = 255;
}

int	apply_light(int color, double *coef)
{
	t_color		color_c;

	color_c = convert_color2c(color);
	color_c.red *= coef[0];
	color_c.green *= coef[1];
	color_c.blue *= coef[2];
	fix_color(&color_c);
	return (convert_color2i(color_c));
}

void	add_coeficient(double *rgb, double coef, int color)
{
	t_color	color_c;

	color_c = convert_color2c(color);
	rgb[0] += coef * color_c.red / 255;
	rgb[1] += coef * color_c.green / 255;
	rgb[2] += coef * color_c.blue / 255;
}
