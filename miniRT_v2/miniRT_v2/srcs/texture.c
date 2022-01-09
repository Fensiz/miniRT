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
//		coords = vector_sub(inter->point, figure->figure.cy.center);
//		theta = atan2(coords.x, coords.z);
//		phi = acos((double)coords.y / figure->figure.cy.radius);
//		//coords = vector_sum(coords, figure->figure.cy.center);
//		raw_u = theta / (2 * M_PI);
//		u = 1 - (raw_u + 0.5);
//		v = fabs(coords.y - (int)coords.y);//1 - phi / M_PI;
//
//		val.x = (int)floor(u * 20) % 2;
//		val.y = (int)floor(v * 10) % 2;
//		party_mix = ((int)val.x ^ (int)val.y);
//		return (party_mix ? BLACK : WHITE);
		coords = vector_sub(inter->point, figure->figure.cy.center);
		coords = vector_x_rot(coords, -(asin(figure->figure.cy.nv.x) / M_PI)*180);
		coords = vector_y_rot(coords, (acos(figure->figure.cy.nv.y) / M_PI)*180);
		coords = vector_z_rot(coords, -(asin(figure->figure.cy.nv.z) / M_PI)*180);
		theta = atan2(coords.x, coords.z);
		phi = acos((double)coords.y / figure->figure.cy.radius);
		//coords = vector_sum(coords, figure->figure.cy.center);
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = (int)coords.y;//fabs(coords.y - (int)coords.y);//1 - phi / M_PI;
		
		
		val.x = (int)floor(u * figure->figure.cy.height) % 2;
		val.y = abs((int)(v*figure->figure.cy.radius/15)) % 2;
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
	else// if (figure->type == PLANE)
	{
		coords.x = (int)floor(fabs(inter->point.x));
		coords.y = (int)floor(fabs(inter->point.y)+0.001);
		coords.z = (int)floor(fabs(inter->point.z));
		val.x = (int)(coords.x) % 2;
		val.y = (int)(coords.y) % 2;
		val.z = (int)(coords.z) % 2;
		party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
		return (party_mix ? BLACK : WHITE);
	}
	//return (party_mix ? BLACK : WHITE);
}

void	apply_texture(t_figure *figure, t_inter *inter)
{
	if (figure->texture == 1)
		inter->color = checkerboard(inter, figure);
}
