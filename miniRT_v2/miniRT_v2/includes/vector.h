#ifndef VECTOR_H
# define VECTOR_H
typedef struct	s_vector //t_p3
{
	double	x;
	double	y;
	double	z;
}	t_vector;
typedef struct s_vector_2p
{
	t_vector	origin;
	t_vector	direction;
}	t_vector_2p;
t_vector	vector_set(double x, double y, double z);
t_vector	vector_sum(t_vector a, t_vector b);
t_vector	vector_sub(t_vector a, t_vector b);
double		vector_dot(t_vector a, t_vector b);
t_vector	vector_cross(t_vector a, t_vector b);
double		vector_len(t_vector v);
t_vector	vector_norm(t_vector v);
double		vcos(t_vector a, t_vector b);
double		vsin(t_vector a, t_vector b);
t_vector	vector_mlt(double n, t_vector v);
double		distance(t_vector p1, t_vector p2);
t_vector	vector_x_rot(t_vector v, double degree_angle);
t_vector	vector_y_rot(t_vector v, double degree_angle);
t_vector	vector_z_rot(t_vector v, double degree_angle);
#endif
