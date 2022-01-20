/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:11:26 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 13:11:27 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

typedef struct s_color
{
	int		red;
	int		green;
	int		blue;
}			t_color;

t_color	set_color(int red, int green, int blue);
int		convert_color2i(t_color	color);
t_color	convert_color2c(int i_color);
int		color_mlt(int i_color, double mlt);
int		color_sum(int i_color_1, int i_color_2);
int		apply_light(int color, double *coef);
void	add_coeficient(double *rgb, double coef, int color);
#endif
