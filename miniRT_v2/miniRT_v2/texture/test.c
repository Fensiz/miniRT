#include <math.h>
#include <stdio.h>

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

t_vector	vector_set(double x, double y, double z)
{
	t_vector	vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return (vector);
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
double	rot_angle(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + coord * coord)) / M_PI) * 180);
}
double	rot_angle2(t_vector n, double coord)
{
	return ((asin(coord / sqrt(n.y * n.y + n.x * n.x + n.z * n.z)) / M_PI) * 180);
}
t_vector	rot_from_y1_to_n(t_vector v, t_vector n)
{
	t_vector tn;
	//printf("%lf,",vector_len(v));
	tn = n;
	printf("n=%lf,%lf,%lf\n", v.x, v.y, v.z);
	if (n.y >= 0)
	{
		//n = vector_x_rot(n, -rot_angle(n, n.x));
		v = vector_x_rot(v, rot_angle2(n, n.z));
		//asin(1/sqrt(3)) /M_PI * 180);//rot_angle(n, n.z));//
		
	}
	else
	{
		v = vector_x_rot(v, 180 - rot_angle2(n, n.z));
		//n = vector_x_rot(n, 180 + rot_angle(n, n.z));
	}
	printf("n=%lf,%lf,%lf\n", v.x, v.y, v.z);
	n = tn;
	if (n.y >= 0)// && n.x >= 0)
		v = vector_z_rot(v, -rot_angle(n, n.x));//rot_angle2(n, n.x));
//	else if (n.y >= 0 && n.x >= 0)
//		v = vector_z_rot(v, -rot_angle(n, n.x));
	else
		v = vector_z_rot(v, 180 + rot_angle(n, n.x));
	//printf("%lf\n",vector_len(v));
	return (v);
}

int main()
{
    t_vector    v;
    t_vector    n;

    v = vector_set(-1,0,0);
    n = vector_set(0,0,-1);
    v = rot_from_y1_to_n(v, n);
    printf("%lf,%lf,%lf\n", v.x, v.y, v.z);
}