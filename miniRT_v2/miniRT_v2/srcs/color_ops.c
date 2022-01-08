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

void	fix_color(t_color	*color)
{
	if (color->red > 255)
		color->red = 255;
	if (color->green > 255)
		color->green = 255;
	if (color->blue > 255)
		color->blue = 255;
}

int	color_mlt(int i_color, double mlt)	//cproduct
{
	t_color	color;

	color = convert_color2c(i_color);
	color.red *= mlt;
	color.green *= mlt;
	color.blue *= mlt;
	return (convert_color2i(color));
}

int	color_sum(int i_color_1, int i_color_2)	//cadd
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

int		color_x_light_new(int color, double *coef)
{
	t_color		color_c;

	color_c = convert_color2c(color);
	color_c.red *= coef[0];
	color_c.green *= coef[1];
	color_c.blue *= coef[2];
	fix_color(&color_c);
	return (convert_color2i(color_c));
}


