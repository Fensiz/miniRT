#include "minirt.h"

static double	cap_intersection(t_vector o, t_vector d, t_figure *lst)
{
	double		id1;
	t_vector	ip1;
	t_vector_2p	ray;

	ray = vector_2p_set(o, d);
	id1 = solve_plane(ray, lst->figure.co.center, lst->figure.co.nv);
	if (id1 < INFINITY)
	{
		ip1 = vector_sum(o, vector_mlt(id1, d));
		if (id1 < INFINITY && distance(ip1, lst->figure.co.center) <= lst->figure.co.radius)
			return (id1);
		return (INFINITY);
	}
	return (INFINITY);
}

double cone_solve(t_vector origin, t_vector direction, t_vector *normal, t_figure *lst)
{
	t_vector center = vector_sum(lst->figure.co.center, vector_mlt(lst->figure.co.height, lst->figure.co.nv));
	t_vector co = vector_sub(origin, center);
	t_vector v = vector_mlt(-1, lst->figure.co.nv);
	double theta = atan(lst->figure.co.radius / lst->figure.co.height);
	double	k[3];
	double	x2[2];

	k[0] = pow(vector_dot(direction, v), 2) - pow(cos(theta), 2);
	k[1] = 2 * (vector_dot(direction, v) * vector_dot(co, v) - vector_dot(direction, co) * pow(cos(theta), 2));
	k[2] = pow(vector_dot(co, v), 2) - vector_dot(co,co) * pow(cos(theta), 2);
	
	double det = pow(k[1], 2) - 4 * k[0] * k[2];
	if (det < 0)
		return (INFINITY);

	x2[0] = (-k[1] - sqrt(det)) / (2 * k[0]);
	x2[1] = (-k[1] + sqrt(det)) / (2 * k[0]);

	// This is a bit messy; there ought to be a more elegant solution.
	double x = x2[0];
	if (x < EPSILON || (x2[1] > EPSILON && x2[1] < x)) x = x2[1];
	if (x < EPSILON) return (INFINITY);

	t_vector cp = vector_sub(vector_sum(origin, vector_mlt(x, direction)), center);
	double h = vector_dot(cp, v);
	if (h < EPSILON || h > lst->figure.co.height) return (INFINITY);

	*normal = vector_norm(vector_sub(vector_mlt(vector_dot(v, cp) / vector_dot(cp, cp), cp), v));
	return x;

}
static double	co_intersection(t_vector origin, t_vector direction, t_vector *normal, t_figure *lst)
{
	return cone_solve(origin, direction, normal, lst);
}

double cone_intersection(t_vector origin, t_vector direction, t_figure *lst)
{
	double		cone_inter;
	double		cap_inter;
	t_vector	co_normal;

	cone_inter = co_intersection(origin, direction, &co_normal, lst);
	cap_inter = cap_intersection(origin, direction, lst);
	if (cone_inter < INFINITY || cap_inter < INFINITY)
	{
		if (cone_inter < cap_inter)
		{
			lst->normal = co_normal;
			return (cone_inter);
		}
		lst->normal = lst->figure.cy.nv;
		return (cap_inter);
	}
	return (INFINITY);
	
	//return cone_solve(origin, direction, lst);
}
