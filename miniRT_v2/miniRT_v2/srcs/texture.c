#include "minirt.h"
#include <stdio.h>

double	rot_angle(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + coord * coord)) / M_PI) * 180);
}

t_vector	rot_form_n_to_y1(t_vector v, t_vector n)
{
	if (n.y >= 0)
	{
		v = vector_x_rot(v, -rot_angle(n, n.z));
		n = vector_x_rot(n, -rot_angle(n, n.z));
	}
	else
	{
		v = vector_x_rot(v, 180 + rot_angle(n, n.z));
		n = vector_x_rot(n, 180 + rot_angle(n, n.z));
	}
	if (n.y >= 0)
		v = vector_z_rot(v, rot_angle(n, n.x));
	else
		v = vector_z_rot(v, 180 - rot_angle(n, n.x));
	return (v);
}

t_vector	rot_from_y1_to_n(t_vector v, t_vector n)
{
	
	if (n.y >= 0)
		v = vector_x_rot(v, rot_angle(n, n.z));
	else
		v = vector_x_rot(v, 180 - rot_angle(n, n.z));
	if (n.y >= 0)
		v = vector_z_rot(v, -rot_angle(n, n.x));
	else
		v = vector_z_rot(v, 180 + rot_angle(n, n.x));
	return (v);
}

t_vector	uv_to_normal(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;
	t_vector	g;

	u *= 400;
	v *= 200;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(v) % map_size + map_size) % map_size;
//	g.x = map[(ui) * map_size + vi] - map[(ui + 1) * map_size + vi];
	g.x = map[(ui - 1 + map_size) % map_size * map_size + vi]
		- map[(ui + 1) 			  % map_size * map_size + vi];
	g.y = 1;
	g.z =
	//map[ui * map_size + vi] - map[ui * map_size + vi + 1];
	map[ui * map_size + (vi - 1 + map_size) % map_size]
		- map[ui * map_size + (vi + 1) 		   % map_size];
	g = vector_norm(g);
	return (g);
}
//g.x = (x[ ((int)floor(u)%40 - 1+40)%40]
//	    [(((int)      v %40)+40)%40]
//	 - x[(((int)floor(u) + 1)%40+40)%40]
//	    [ ((int)floor(v)%40+40)%40]);
//		g.y = 1;
//		g.z = (x[((int)floor(u)%40+40)%40][((int)floor(v)%40 - 1+40)%40]    - x[((int)floor(u)%40+40)%40][((int)floor(v)%40 + 1+40)%40]);

int	texture_plane(t_vector point)
{
	t_vector	coords;

	coords.x = abs((int)floor((point.x) + EPSILON));
	coords.y = abs((int)floor((point.y) + EPSILON));
	coords.z = abs((int)floor((point.z) + EPSILON));
	coords.x = (int)(coords.x) % 2;
	coords.y = (int)(coords.y) % 2;
	coords.z = (int)(coords.z) % 2;
	if (((int)coords.x ^ (int)coords.y) ^ (int)coords.z)
		return (BLACK);
	else
		return (WHITE);
}

static int	checkerboard(t_inter *inter, t_figure *figure)
{
	int x[1600] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
		0,0,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,
		0,0,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,0,0,
		0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
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


		coords = rot_form_n_to_y1(coords, figure->figure.cy.nv);
		theta = atan2(coords.x, coords.z);
		
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = coords.y;
		r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
		val.x = abs((int)floor(theta / M_PI * figure->figure.cy.radius * 4)) % 2;
		val.y = abs((int)floor(v)) % 2;
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
			return (texture_plane(coords));
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

		coords = rot_form_n_to_y1(coords, figure->figure.cy.nv);
		theta = atan2(coords.x, coords.z);
		
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = coords.y;
		r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
		val.x = abs((int)floor(theta / M_PI * figure->figure.cy.radius * 4)) % 2;
		val.y = abs((int)floor(v)) % 2;
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
		{
			return (texture_plane(coords));
//			coords.x = abs((int)floor((coords.x) + 10*EPSILON));
//			coords.y = abs((int)floor((coords.y) + 10*EPSILON));
//			coords.z = abs((int)floor((coords.z) + 10*EPSILON));
//			val.x = (int)(coords.x) % 2;
//			val.y = (int)(coords.y) % 2;
//			val.z = (int)(coords.z) % 2;
//			party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
		//	party_mix = ((int)val.x ^ (int)val.y) ^ (int)r; //аналог для плоских частей конуса
		}
		else
			party_mix = ((int)val.x ^ (int)val.y);
		return (party_mix ? BLACK : WHITE);
	}
	else if (figure->type == SPHERE)
	{
		

		t_vector g;
		//---------------------------------
		coords = vector_sub(inter->point, figure->figure.sp.center);
		theta = atan2(coords.x, coords.z);
		phi = acos((double)coords.y / figure->figure.sp.radius);
		coords = vector_sum(coords, figure->figure.sp.center);
		raw_u = theta / (2 * M_PI);
		u = 1 - (raw_u + 0.5);
		v = 1 - phi / M_PI;
		
		//---------------------------------
//		u *= 400;
//		v *=200;
//		g.x = (x[((int)floor(u)%40 - 1+40)%40][(((int)v%40)+40)%40] - x[(((int)floor(u) + 1)%40+40)%40][((int)floor(v)%40+40)%40]);
//		g.y = 1;
//		g.z = (x[((int)floor(u)%40+40)%40][((int)floor(v)%40 - 1+40)%40]    - x[((int)floor(u)%40+40)%40][((int)floor(v)%40 + 1+40)%40]);
//
//		g = vector_norm(g);
//
		g = uv_to_normal(u, v, x, 40);
		g = rot_from_y1_to_n(g, inter->normal);
		inter->normal = vector_norm(vector_sum(inter->normal, g));
		//-----------------------------
		
		
//		u /= 400;
//		v /= 200;
		val.x = (int)floor(u * 20) % 2;
		val.y = (int)floor(v * 10) % 2;
		party_mix = ((int)val.x ^ (int)val.y);
		
		return (party_mix ? BLACK : WHITE);
	}
	else
	{
		return (texture_plane(inter->point));
//		coords.x = abs((int)floor((inter->point.x) + EPSILON));
//		coords.y = abs((int)floor((inter->point.y) + EPSILON));
//		coords.z = abs((int)floor((inter->point.z) + EPSILON));
//		val.x = (int)(coords.x) % 2;
//		val.y = (int)(coords.y) % 2;
//		val.z = (int)(coords.z) % 2;
//		party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
//
	}

}

void	apply_texture(t_figure *figure, t_inter *inter)
{
	if (figure->texture == 1)
		inter->color = checkerboard(inter, figure);
}
