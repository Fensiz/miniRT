
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
double	rot_angle3(t_vector n, double coord)
{
	return (( asin(-n.x/sqrt(n.z*n.z + n.x*n.x)) / M_PI) * 180);
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

	 if (n.x >= 0 && n.y >= 0 && n.z < 0)
	 {
		v = vector_z_rot(v, -rot_angle2(n, n.x));
		v = vector_x_rot(v, rot_angle(n, n.z));
	 }
	else if (n.x < 0 && n.y >= 0 && n.z < 0)
	{
	   v = vector_z_rot(v, -rot_angle2(n, n.x));
	   v = vector_x_rot(v, rot_angle(n, n.z));
	}
	else if (n.x >= 0 && n.y < 0 && n.z < 0)
	{
	   v = vector_z_rot(v, -rot_angle2(n, n.x));
	   v = vector_x_rot(v, 180 - rot_angle(n, n.z));
	}
	else if (n.x < 0 && n.y < 0 && n.z < 0)
	{
	   v = vector_z_rot(v, -rot_angle2(n, n.x));
	   v = vector_x_rot(v, 180 - rot_angle(n, n.z));
	}
	return (v);
}

int	uv_height(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;

	u *= 800;
	v *= 400;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	return (map[ui + vi * map_size]);
}

t_vector	uv_to_normal(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;
	t_vector	g;

	u *= 800;
	v *= 400;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	g.x = (map[(ui - 1 + map_size) % map_size + vi* map_size]
		- map[(ui + 1) % map_size + vi * map_size]);
	g.y = EPSILON;
	g.z = -(map[ui + (vi - 1 + map_size) % map_size * map_size]
		 - map[ui + (vi + 1) % map_size * map_size]);
	if (g.x == 0 && g.z == 0)
		return vector_set(0, 0, 0);
	g = vector_norm(g);
	return (g);
}
t_vector	uv_to_normal_pl(double u, double v, int *map, int map_size)
{
	int	ui;
	int	vi;
	t_vector	g;

	
	u *= 20;
	v *= 20;
	ui = ((int)floor(u) % map_size + map_size) % map_size;
	vi = ((int)floor(-v) % map_size + map_size) % map_size;
	g.x = (map[(ui - 1 + map_size) % map_size + vi* map_size]
		- map[(ui + 1) % map_size + vi * map_size]);
	g.y = EPSILON;
	g.z = -(map[ui + (vi - 1 + map_size) % map_size * map_size]
		 - map[ui + (vi + 1) % map_size * map_size]);
	if (g.x == 0 && g.z == 0)
		return vector_set(0, 0, 0);
	g = vector_norm(g);
	return (g);
}


void	texture_plane(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;
	t_vector	gx;

	coords = rot_form_n_to_y1(inter->point, inter->normal);
	if (figure->texture >> 1)
	{
		gx = uv_to_normal_pl(coords.x, coords.z, map->map, map->size);
		if (vector_len(gx))
		{
			gx = vector_mlt(0.2, gx);
			gx = vector_sum(gx, vector_set(0, 1, 0));
			gx = vector_norm(gx);
			inter->normal = rot_from_y1_to_n(gx, inter->normal);
		}
	}
	if (figure->texture & 1)
	{
		coords.x = abs((int)floor((coords.x) + EPSILON));
		coords.y = abs((int)floor((coords.y) + EPSILON));
		coords.z = abs((int)floor((coords.z) + EPSILON));
		coords.x = (int)(coords.x) % 2;
		coords.y = (int)(coords.y) % 2;
		coords.z = (int)(coords.z) % 2;
		if (((int)coords.x) ^ (int)coords.z)
			inter->color = BLACK;
		else
			inter->color = WHITE;
	}
}
void	texture_cone(t_figure *figure, t_inter *inter, t_map *map)
{
	t_vector	coords;
	t_vector	gx;
	t_uv		i;
	double		theta;
	t_vector	val;
		
	coords = vector_sub(inter->point, figure->figure.co.center);
	coords.x += EPSILON;
	coords.y += EPSILON;
	coords.z += EPSILON;
	coords = rot_form_n_to_y1(coords, figure->figure.co.nv);
	theta = atan2(coords.x, coords.z);
	i.u = theta / M_PI * figure->figure.co.radius * 4;
	i.v = coords.y;

	if (figure->texture >> 1)
	{
		gx = uv_to_normal_pl(i.u , i.v, map->map, map->size);
		if (vector_len(gx))
		{
			gx = vector_mlt(0.2, gx);
			gx = vector_sum(gx, vector_set(0, 1, 0));
			gx = vector_norm(gx);
			inter->normal = rot_from_y1_to_n(gx, inter->normal);
		}
	}
	if (figure->texture & 1)
	{
		val.x = abs((int)floor(i.u)) % 2;
		val.y = abs((int)floor(i.v)) % 2;
		if (((int)val.x) ^ (int)val.y)
			inter->color = BLACK;
		else
			inter->color = WHITE;
	}
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
	t_vector	gx;
	t_uv		i;
	t_vector	val;
	int height;

	
	i = uv_sphere(inter, figure);
	if (figure->texture >> 1)
	{
		height = uv_height(i.u, i.v, map->map, map->size);
		gx = uv_to_normal(i.u, i.v, map->map, map->size);
		if (vector_len(gx))
		{
			gx = vector_mlt(0.2, gx);
			gx = vector_sum(gx, vector_set(0, 1, 0));
			gx = vector_norm(gx);
			inter->normal = rot_from_y1_to_n(gx, inter->normal);
		}
	}
	
	if (figure->texture & 1)
	{
		val.x = (int)floor(i.u * 40) % 2;
		val.y = (int)floor(i.v * 20) % 2;
		if ((int)val.x ^ (int)val.y)
			inter->color = BLACK;
		else
			inter->color = WHITE;
	}
}

static int	checkerboard(t_inter *inter, t_figure *figure, t_scene *scene)
{
	t_vector	coords;
	t_vector	val;
	int		party_mix;
	double theta;
	double u, v, r;

	if (figure->type == CYLINDER)
	{
		coords = vector_sub(inter->point, figure->figure.cy.center);
		coords.x += EPSILON;
		coords.y += EPSILON;
		coords.z += EPSILON;

		coords = rot_form_n_to_y1(coords, figure->figure.cy.nv);
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.cy.height) < 0.01)
		{
			texture_plane(figure, inter, scene->map);
			return (inter->color);
		}
		else
		{
			theta = atan2(coords.x, coords.z);
			u = 1 - (theta / (2 * M_PI) + 0.5);
			v = coords.y;
			r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
			val.x = abs((int)floor(theta / M_PI * figure->figure.cy.radius * 4)) % 2;
			val.y = abs((int)floor(v)) % 2;
			party_mix = ((int)val.x ^ (int)val.y);
		}
		return (party_mix ? BLACK : WHITE);
	}
	else if (figure->type == CONE)
	{
		coords = vector_sub(inter->point, figure->figure.co.center);
		coords.x += EPSILON;
		coords.y += EPSILON;
		coords.z += EPSILON;
		coords = rot_form_n_to_y1(coords, figure->figure.co.nv);
		theta = atan2(coords.x, coords.z);
		
		//u = 1 - (theta / (2 * M_PI) + 0.5);
		//v = coords.y;
		//r = (int)sqrt(pow(coords.x, 2) + pow(coords.z, 2))%2 ^ 1;
		
		if (fabs(coords.y) < 0.01 || fabs(fabs(coords.y) - figure->figure.co.height) < 0.01)
		{
			texture_plane(figure, inter, scene->map);
			return (inter->color);
		}
		else
		{
//			u = theta / M_PI * figure->figure.co.radius * 4;
//			v = coords.y;
//			val.x = abs((int)floor(u)) % 2;
//			val.y = abs((int)floor(v)) % 2;
//			party_mix = ((int)val.x ^ (int)val.y);
			texture_cone(figure, inter, scene->map);
			return (inter->color);
		}
		//return (party_mix ? BLACK : WHITE);
	}
	else if (figure->type == SPHERE)
	{
		texture_sphere(inter, figure, scene->map);
		return (inter->color);
	}
	else
	{
		texture_plane(figure, inter, scene->map);
		return (inter->color);
	}

}

void	apply_texture(t_figure *figure, t_inter *inter, t_scene *scene)
{
	if (figure->texture != 0)
		inter->color = checkerboard(inter, figure, scene);
}
