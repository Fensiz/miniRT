#include "minirt.h"
#include <stdio.h>
static int	checkerboard(t_inter *inter, t_figure *figure)
{
	t_vector	coords;
	t_vector	val;
	int		party_mix;
	double theta;
	double phi;
	double raw_u, u, v, r;

	if (figure->type == CYLINDER)
	{
		coords = vector_sub(inter->point, figure->figure.cy.center);
		coords.x += EPSILON;
		coords.y += EPSILON;
		coords.z += EPSILON;

		 t_vector n;
		 n = figure->figure.cy.nv;
		//printf("=%lf/%lf/%lf---%lf\n",n.x, n.y, n.z, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 
		 if (n.y >= 0)
		 {
			 coords = vector_x_rot(coords, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
			 n = vector_x_rot(n, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 }
		 else
		 {
			 coords = vector_x_rot(coords, 180 + (asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
			 n = vector_x_rot(n, 180 + (asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 }
		 if (n.y >= 0)
		 {
			 coords = vector_z_rot(coords, (asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
			 n = vector_z_rot(n, (asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
		 }
		 else
		 {
			 coords = vector_z_rot(coords, 180-(asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
			 n = vector_z_rot(n, 180-(asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
		 }

		theta = atan2(coords.x, coords.z);
		
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = coords.y;
		r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
		//val.x = abs((int)floor(theta / M_PI * 180 / (M_PI * figure->figure.cy.radius))) % 2;
		val.x = abs((int)floor(theta / M_PI * figure->figure.cy.radius * 4)) % 2;
		val.y = abs((int)floor(v)) % 2;
//		if (coords.y > figure->figure.cy.height - 3)
//		printf("%lf %lf\n", coords.y, figure->figure.cy.height);
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
		{
			coords.x = abs((int)floor((coords.x) + 10*EPSILON));
			coords.y = abs((int)floor((coords.y) + 10*EPSILON));
			coords.z = abs((int)floor((coords.z) + 10*EPSILON));
			val.x = (int)(coords.x) % 2;
			val.y = (int)(coords.y) % 2;
			val.z = (int)(coords.z) % 2;
			party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
		//	party_mix = ((int)val.x ^ (int)val.y) ^ (int)r; //аналог для плоских частей конуса
		}
		else
			party_mix = ((int)val.x ^ (int)val.y);
		return (party_mix ? BLACK : WHITE);
	}
	else if (figure->type == CONE)
	{
		coords = vector_sub(inter->point, figure->figure.co.center);
		coords.x += EPSILON;
		coords.y += EPSILON;
		coords.z += EPSILON;

		 t_vector n;
		 n = figure->figure.co.nv;
		//printf("=%lf/%lf/%lf---%lf\n",n.x, n.y, n.z, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 
		 if (n.y >= 0)
		 {
			 coords = vector_x_rot(coords, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
			 n = vector_x_rot(n, -(asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 }
		 else
		 {
			 coords = vector_x_rot(coords, 180 + (asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
			 n = vector_x_rot(n, 180 + (asin(n.z/sqrt(n.y * n.y + n.z * n.z)) / M_PI) * 180);
		 }
		 if (n.y >= 0)
		 {
			 coords = vector_z_rot(coords, (asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
			 n = vector_z_rot(n, (asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
		 }
		 else
		 {
			 coords = vector_z_rot(coords, 180-(asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
			 n = vector_z_rot(n, 180-(asin(n.x/sqrt(n.x * n.x + n.y * n.y)) / M_PI) * 180);
		 }

		theta = atan2(coords.x, coords.z);
		
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = coords.y;
		r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
		//val.x = abs((int)floor(theta / M_PI * 180 / (M_PI * figure->figure.cy.radius))) % 2;
		val.x = abs((int)floor(theta / M_PI * figure->figure.cy.radius * 4)) % 2;
		val.y = abs((int)floor(v)) % 2;
//		if (coords.y > figure->figure.cy.height - 3)
//		printf("%lf %lf\n", coords.y, figure->figure.cy.height);
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
		{
			coords.x = abs((int)floor((coords.x) + 10*EPSILON));
			coords.y = abs((int)floor((coords.y) + 10*EPSILON));
			coords.z = abs((int)floor((coords.z) + 10*EPSILON));
			val.x = (int)(coords.x) % 2;
			val.y = (int)(coords.y) % 2;
			val.z = (int)(coords.z) % 2;
			party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
		//	party_mix = ((int)val.x ^ (int)val.y) ^ (int)r; //аналог для плоских частей конуса
		}
		else
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
