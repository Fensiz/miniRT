/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 12:18:34 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 12:18:35 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "color.h"

t_color	set_color(int red, int green, int blue)
{
	t_color	color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return (color);
}

int	convert_color2i(t_color	color)
{
	return ((color.red << 16) + (color.green << 8) + color.blue);
}

t_color	convert_color2c(int i_color)
{
	t_color	color;

	color.red = (i_color >> 16) & 0xFF;
	color.green = (i_color >> 8) & 0xFF;
	color.blue = i_color & 0xFF;
	return (color);
}

int	color_mlt(int i_color, double mlt)
{
	t_color	color;

	color = convert_color2c(i_color);
	color.red *= mlt;
	color.green *= mlt;
	color.blue *= mlt;
	return (convert_color2i(color));
}
