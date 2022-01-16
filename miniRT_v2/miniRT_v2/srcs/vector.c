#include "vector.h"
#include <math.h>

t_vector	vector_set(double x, double y, double z)
{
	t_vector	vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return (vector);
}

t_vector	vector_sum(t_vector a, t_vector b)
{
	t_vector	ret;

	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return (ret);
}

t_vector	vector_sub(t_vector a, t_vector b)
{
	t_vector	ret;

	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return (ret);
}

double	vector_dot(t_vector a, t_vector b) //mlt_scalar //скалярное произведение //DotProduct
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector	vector_cross(t_vector a, t_vector b) //векторное произведение //CrossProduct
{
	t_vector	ret;

	ret.x = a.y * b.z - a.z * b.y;
	ret.y = a.z * b.x - a.x * b.z;
	ret.z = a.x * b.y - a.y * b.x;
	return (ret);
}

double	vector_len(t_vector v) //модуль/длина вектора
{
	return (sqrt(fabs(vector_dot(v, v))));
}

t_vector	vector_norm(t_vector v)
{
	t_vector	ret;
	double		len;

	len = vector_len(v);
	ret.x = v.x / len;
	ret.y = v.y / len;
	ret.z = v.z / len;
	return (ret);
}

double	vcos(t_vector a, t_vector b) //косинус угола между векторами
{
	return (vector_dot(a, b) / (vector_len(a) * vector_len(b)));
}

double	vsin(t_vector a, t_vector b)
{
	return (sqrt(1 - pow(vcos(a, b), 2)));
}

t_vector	vector_mlt(double n, t_vector v) // умножение вектора на число
{
	t_vector	ret;

	ret.x = n * v.x;
	ret.y = n * v.y;
	ret.z = n * v.z;
	return (ret);
}

double	distance(t_vector p1, t_vector p2) // расстояние между двумя точками
{
	double	ret;

	ret = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
	return (ret);
}

t_vector	vector_x_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(1, 0, 0);
	rot_mtx[1] = vector_set(0, cos(rad_angle), -sin(rad_angle));
	rot_mtx[2] = vector_set(0, sin(rad_angle), cos(rad_angle));
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector	vector_y_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(cos(rad_angle), 0, sin(rad_angle));
	rot_mtx[1] = vector_set(0, 1, 0);
	rot_mtx[2] = vector_set(-sin(rad_angle), 0, cos(rad_angle));
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector	vector_z_rot(t_vector v, double degree_angle)
{
	t_vector	rot_mtx[3];
	t_vector	ret;
	double		rad_angle;

	rad_angle = degree_angle * M_PI / 180;
	rot_mtx[0] = vector_set(cos(rad_angle), -sin(rad_angle), 0);
	rot_mtx[1] = vector_set(sin(rad_angle), cos(rad_angle), 0);
	rot_mtx[2] = vector_set(0, 0, 1);
	ret.x = v.x * rot_mtx[0].x + v.y * rot_mtx[0].y + v.z * rot_mtx[0].z;
	ret.y = v.x * rot_mtx[1].x + v.y * rot_mtx[1].y + v.z * rot_mtx[1].z;
	ret.z = v.x * rot_mtx[2].x + v.y * rot_mtx[2].y + v.z * rot_mtx[2].z;
	return (ret);
}

t_vector	vector_id(const int id)
{
	const t_vector	identity[3] = {
		(t_vector){1.0, 0.0, 0.0},
		(t_vector){0.0, 1.0, 0.0},
		(t_vector){0.0, 0.0, 1.0}
	};

	return (identity[id]);
}
