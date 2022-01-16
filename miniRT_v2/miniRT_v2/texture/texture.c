
#include "minirt.h"
#include <stdio.h>

double	rot_angle(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + coord * coord)) / M_PI) * 180);
}
double	rot_angle2(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + n.x * n.x + n.z * n.z)) / M_PI) * 180);
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
	//printf("%lf,",vector_len(v));

	t_vector s = v;
//	if (n.y >= 0 && n.x >=0 && n.z <0)
//	{
		v = vector_z_rot(v, rot_angle2(n, n.x));
		v = vector_x_rot(v, rot_angle(n, n.z));
//	}
	
	v.x *= -1;
	if (n.y <0)
		v.y *= -1;
	//v.z = 0;
//	v.z *= 2;
//	if (n.y < 0 && n.x >=0 && n.z <0)
//	{
//		v = vector_z_rot(v, rot_angle2(n, n.x));
//		v = vector_x_rot(v, rot_angle(n, n.z));
//		//v.z *= -1;
//		//v.x *= -1;
//	}
//	else if (n.y >= 0 && n.x < 0 && n.z <0)
//	{
//		v = vector_z_rot(v, rot_angle2(n, n.x));
//		v = vector_x_rot(v, rot_angle(n, n.z));
//	}
//	else if (n.y < 0 && n.x < 0 && n.z <0)
//	{
//		v = vector_z_rot(v, rot_angle2(n, n.x));
//		v = vector_x_rot(v, rot_angle(n, n.z));
//	}
//	if (n.y >= 0)// && n.x >= 0)
//		v = vector_z_rot(v, rot_angle2(n, n.x));
//	if (n.y >= 0)// && n.z <0)
//		//v = vector_z_rot(v, rot_angle2(n, n.x));//
//	v = vector_x_rot(v, -rot_angle(n, n.z));
	
		//n = vector_x_rot(n, -rot_angle(n, n.z));
//	else if(n.y >=0)
//		v = vector_x_rot(v, rot_angle2(n, n.z));
//	else
//	{
//		//v = vector_z_rot(v, rot_angle2(n, n.x));
//		v = vector_x_rot(v, rot_angle2(n, n.z));
//		//v.x *= -1;
//
//		//n = vector_x_rot(n, 180 + rot_angle(n, n.z));
//	}
	
//		v = vector_x_rot(v, -rot_angle(n, n.z));//-
//	else if (n.y >= 0 && n.x < 0)
//		v = vector_z_rot(v, -rot_angle(n, n.x));
//	else
//	{
//		v = vector_z_rot(v, -rot_angle2(n, n.x));
////		v = vector_x_rot(v, -rot_angle(n, n.z));//-
//		//v.z *= -1;

//
//	}
	//v = vector_y_rot(v,180);
	
	if (n.y>=0&&n.z<0 && n.x>=0)
	printf("VS%.2lf,%.2lf,%.2lf _ V%.2lf,%.2lf,%.2lf _ N%.2lf,%.2lf,%.2lf\n",s.x,s.y,s.z,v.x,v.y,v.z,n.x,n.y,n.z);
	return (v);
}

t_vector	uv_to_normal(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;
	t_vector	g;

	u *= 1000;
	v *= 500;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	g.x = 0;//-(map[(ui - 1 + map_size) % map_size + vi* map_size]
//		- map[(ui + 1) % map_size + vi * map_size]);
	g.y = 1;
	g.z = 0;// -(map[ui + (vi - 1 + map_size) % map_size * map_size]
//		 - map[ui + (vi + 1) % map_size * map_size]);
//	if (g.x == 0 && g.z == 0)
//		return vector_set(0, 0, 0);
	g = vector_norm(g);
	//g.y *= -1;
	//g = vector_set(0, 0, -1);
	return (g);
}
t_vector	uv_to_normal2(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;
	t_vector	g;

	u *= 600;
	v *= 300;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	g.x =  0;//(map[(ui - 1 + map_size) % map_size + vi* map_size]
//		- map[(ui + 1) % map_size + vi * map_size]);
	g.y = 0;
	g.z =  0;//-(map[ui + (vi - 1 + map_size) % map_size * map_size]
//		 - map[ui + (vi + 1) % map_size * map_size]);
	if (g.x == 0 && g.z == 0)
		return vector_set(0, 0, 0);
	g = vector_norm(g);
	//g.y *= -1;
	//g = vector_set(0, 0, -1);
	return (g);
}


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

t_uv	uv_sphere(t_inter *inter, t_figure *figure)
{
	t_vector	coords;
	double	phi;
	double	theta;
	t_uv	i;

	coords = vector_sub(inter->point, figure->figure.sp.center);
	theta = atan2(coords.x, coords.z);
	phi = acos((double)coords.y / figure->figure.sp.radius);
	coords = vector_sum(coords, figure->figure.sp.center);
	i.u = 1 - (theta / (2 * M_PI) + 0.5);
	i.v = 1 - phi / M_PI;
	return (i);
}

void	texture_sphere(t_inter *inter, t_figure *figure, t_map *map)
{
	t_vector 	g;
	t_vector	gx;
	t_vector	gx2, g2;
	t_uv		i;
	t_vector	val;
	t_vector	temp_n;
	t_vector	temp_n2;

	temp_n = inter->normal;
	temp_n2 = inter->normal;
	i = uv_sphere(inter, figure);
	if (figure->texture >> 1)
	{
		gx = uv_to_normal(i.u, i.v, map->map, map->size);
		if (vector_len(gx))
			g = rot_from_y1_to_n(gx, inter->normal);
		
		temp_n.x += g.x;
		temp_n.y += g.y;
		if (g.x < 0)
		{
			if (inter->normal.x < 0)
				temp_n.z -= g.z;
			else
				temp_n.z += g.z;
		}
		else
		{
			if (inter->normal.x < 0)
				temp_n.z += g.z;
			else
				temp_n.z -= g.z;
		}
//		inter->normal = vector_norm(temp_n);
//		inter->normal = vector_norm(vector_sum(inter->normal, vector_mlt(1, g)));
		
		gx2 = uv_to_normal2(i.u, i.v, map->map, map->size);
		if (vector_len(gx2))
			g2 = rot_from_y1_to_n(gx2, inter->normal);
		temp_n2.x += g2.x;
		if(inter->normal.y >= 0)
			temp_n2.y += g2.y;
		else
			temp_n2.y -= g2.y;
		if (gx2.z < 0)
		temp_n2.z += g2.z;
		else
			temp_n2.z -= g2.z;
		inter->normal = vector_norm(vector_sum(temp_n, temp_n2));
		
//		if (gx.x >= 0)
//		{
//			if (inter->normal.x >= 0)
//			{
//				if (inter->normal.y <= 0)
//					temp_n.y += g.y;
//				else
//					temp_n.y -= g.y;
////				if (gx.x >= 0)
//				if (gx.z >= 0)
//					temp_n.z += g.z;
//
//				else
//					temp_n.z -= g.z;
//			}
//			else
//			{
//				if (inter->normal.y <= 0)
//				temp_n.y -= g.y;
//				else
//					temp_n.y += g.y;
////				if (inter->normal.y >=0 && g.y < 0 && inter->normal.x < 0)
//
//					//temp_n.z -= g.z;
//			}
//		}
//		else
//		{
//			if (inter->normal.x >= 0)
//			{
//				if (inter->normal.y <= 0)
//					temp_n.y -= g.y;
//				else
//					temp_n.y += g.y;
//				temp_n.z -= g.z;
//			}
//			else
//			{
//				if (inter->normal.y <= 0)
//					temp_n.y += g.y;
//				else
//					temp_n.y -= g.y;
//				temp_n.z += g.z;
//			}
//		}
	//		}
//		else
//		{
//			inter->normal.z += g.z;
//			//inter->normal.x += g.x;
//		}
//
		//Z < 0 -y +z
		//inter->normal.x += g.x;// всегда
//		inter->normal.y -= g.y;
//		inter->normal.z += g.z; // меняет верх низ -для полож z
//		inter->normal.x += g.x;
//		if (inter->normal.y <= 0)
//			inter->normal.y -= g.y;
//		else
//			inter->normal.y += g.y;
//		if (inter->normal.x < 0)
//		{
//			if (g.x >= 0)
//				inter->normal.z -= g.z;
////				else
////					inter->normal.z += g.z;
//
//			//inter->normal.z -= g.z;
//		}
		
		
//		inter->normal = vector_norm(vector_sum(inter->normal, vector_mlt(1, g)));
		
		//		else
//			inter->normal = vector_norm(vector_sum(inter->normal, vector_mlt(1, g)));
	}
	val.x = (int)floor(i.u * 40) % 2;
	val.y = (int)floor(i.v * 20) % 2;
	if (figure->texture & 1)
	{
		if ((int)val.x ^ (int)val.y)
			inter->color = BLACK;
		else
			inter->color = WHITE;
	}
}

static int	checkerboard(t_inter *inter, t_figure *figure, t_scene *scene)
{
	int y[] = {
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
				0,0,1,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,1,0,0,
				0,0,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,1,0,0,
				0,0,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,1,0,0,
				0,0,1,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,1,0,0,
				0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		
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
		texture_sphere(inter, figure, scene->map);
		return (inter->color);
//		t_vector g;
//
//		coords = vector_sub(inter->point, figure->figure.sp.center);
//		theta = atan2(coords.x, coords.z);
//		phi = acos((double)coords.y / figure->figure.sp.radius);
//		coords = vector_sum(coords, figure->figure.sp.center);
//		u = 1 - (theta / (2 * M_PI) + 0.5);
//		v = 1 - phi / M_PI;
//		if (figure->texture >> 1)
//		{
//			g = rot_from_y1_to_n(uv_to_normal(u, v, y, 40), inter->normal);
//			inter->normal = vector_norm(vector_sum(inter->normal, g));
//		}
//		val.x = (int)floor(u * 40) % 2;
//		val.y = (int)floor(v * 20) % 2;
//		party_mix = ((int)val.x ^ (int)val.y);
//		if (figure->texture & 1)
//		{
//			if ((int)val.x ^ (int)val.y)
//				return (BLACK);
//			return (WHITE);
//		}
//		return (inter->color);
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

void	apply_texture(t_figure *figure, t_inter *inter, t_scene *scene)
{
	if (figure->texture != 0)
		inter->color = checkerboard(inter, figure, scene);
}
