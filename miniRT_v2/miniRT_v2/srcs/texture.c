#include "minirt.h"
#include <stdio.h>
static int	checkerboard(t_inter *inter, t_figure *figure)
{
	t_vector	coords;
	t_vector	val;
	int		party_mix;
	double theta;
	double phi;
	double raw_u, u, v;

	if (figure->type == CYLINDER)
	{
		coords = vector_sub(inter->point, figure->figure.cy.center);
		coords.x += EPSILON;
		coords.y += EPSILON;
		coords.z += EPSILON;

		if ((figure->figure.cy.nv.y >= 0 && figure->figure.cy.nv.z >= 0))
			coords = vector_x_rot(coords, -(asin(figure->figure.cy.nv.z) / M_PI) * 180);
		else if (figure->figure.cy.nv.y >= 0 && figure->figure.cy.nv.z < 0)
			coords = vector_x_rot(coords, -(asin(figure->figure.cy.nv.z) / M_PI) * 180);
		else if (figure->figure.cy.nv.y < 0 && figure->figure.cy.nv.z >= 0)
			coords = vector_x_rot(coords, (asin(figure->figure.cy.nv.z) / M_PI) * 180);
		else
			coords = vector_x_rot(coords, (asin(figure->figure.cy.nv.z) / M_PI) * 180);
		if ((figure->figure.cy.nv.y >= 0 && figure->figure.cy.nv.x >= 0))
			coords = vector_z_rot(coords, 180+(asin(figure->figure.cy.nv.x) / M_PI) * 180);
		else if ((figure->figure.cy.nv.y < 0 && figure->figure.cy.nv.x < 0))
			coords = vector_z_rot(coords, -(asin(figure->figure.cy.nv.x) / M_PI) * 180);
		else if ((figure->figure.cy.nv.y >= 0 && figure->figure.cy.nv.x < 0))
			coords = vector_z_rot(coords, 180+(asin(figure->figure.cy.nv.x) / M_PI) * 180);
		else
			coords = vector_z_rot(coords, -(asin(figure->figure.cy.nv.x) / M_PI) * 180);
		theta = atan2(coords.x, coords.z);
		
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = (int)coords.y;
		val.x = abs((int)floor(u * figure->figure.cy.radius*4)) % 2;
		val.y = abs((int)floor(v * figure->figure.cy.height/4)) % 2;
		party_mix = ((int)val.x ^ (int)val.y);
		return (party_mix ? BLACK : WHITE);
	}
	else if (figure->type == SPHERE)
	{
		coords = vector_sub(inter->point, figure->figure.sp.center);
		theta = atan2(coords.x, coords.z);
		phi = acos((double)coords.y / figure->figure.sp.radius);
		coords = vector_sum(coords, figure->figure.sp.center);
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = 1 - phi / M_PI;
		
		val.x = (int)floor(u * 20) % 2;
		val.y = (int)floor(v * 10) % 2;
		party_mix = ((int)val.x ^ (int)val.y);
		return (party_mix ? BLACK : WHITE);
	}
	else
	{
		coords.x = abs((int)floor((inter->point.x) + EPSILON));
		coords.y = abs((int)floor((inter->point.y) + EPSILON));
		coords.z = abs((int)floor((inter->point.z) + EPSILON));
		val.x = (int)(coords.x) % 2;
		val.y = (int)(coords.y) % 2;
		val.z = (int)(coords.z) % 2;
		party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
		return (party_mix ? BLACK : WHITE);
	}
}

void	apply_texture(t_figure *figure, t_inter *inter)
{
	if (figure->texture == 1)
		inter->color = checkerboard(inter, figure);
}
