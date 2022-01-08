#include "minirt.h"
#include <stdio.h>
static void		solve_sphere(double x[2], t_vector origin, t_vector direction, t_figure *figure)
{
	double		discr;
	t_vector	cam_sphere;
	double		k[3];

	cam_sphere = vector_sub(origin, figure->figure.sp.center);  //cam.origin
	k[0] = vector_dot(direction, direction);
	k[1] = 2 * vector_dot(direction, cam_sphere);
	k[2] = vector_dot(cam_sphere, cam_sphere) - figure->figure.sp.radius * figure->figure.sp.radius;
	discr = k[1] * k[1] - (4 * k[0] * k[2]);
	if (discr >= 0)
	if (discr < 0)
	{
		x[0] = INFINITY;
		x[1] = INFINITY;
		return ;
	}
	x[0] = (-k[1] + sqrt(discr)) / (2 * k[0]);
	x[1] = (-k[1] - sqrt(discr)) / (2 * k[0]);
}

double			sphere_intersection(t_vector origin, t_vector direction, t_figure *figure)
{
	double		closest;
	double		x[2];

	closest = INFINITY;
	solve_sphere(x, origin, direction, figure);
	
	if (x[0] > EPSILON && x[0] < INFINITY)
		closest = x[0];
	if (x[1] > EPSILON && x[1] < INFINITY && x[1] < x[0])
		closest = x[1];
	return (closest);
}

double			solve_plane(t_vector origin, t_vector direction, t_vector plane_p, t_vector plane_nv)
{
	double	x;
	double	denom;

	denom = vector_dot(plane_nv, direction);
	if (denom == 0)
		return (INFINITY);
	x = (vector_dot(plane_nv, vector_sub(plane_p, origin))) / denom;
	return (x > 0 ? x : INFINITY);
}

double			plane_intersection(t_vector origin, t_vector d, t_figure *lst)
{
	return (solve_plane(origin, d, lst->figure.pl.point, lst->normal));
}






static int		solve_cylinder(double x[2], t_vector o, t_vector d, t_figure *lst)
{
	t_vector	v;
	t_vector	u;
	double	a;
	double	b;
	double	c;

	v = vector_mlt(vector_dot(d, lst->figure.cy.nv), lst->figure.cy.nv);
	v = vector_sub(d, v);
	u = vector_mlt(vector_dot(vector_sub(o, lst->figure.cy.center), lst->figure.cy.nv),
													lst->figure.cy.nv);
	u = vector_sub(vector_sub(o, lst->figure.cy.center), u);
	a = vector_dot(v, v);
	b = 2 * vector_dot(v, u);
	c = vector_dot(u, u) - pow(lst->figure.cy.radius, 2);
	x[0] = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	x[1] = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	if ((x[0] != x[0] && x[1] != x[1]) || (x[0] < EPSILON && x[1] < EPSILON))
	{
		x[0] = INFINITY;
		x[1] = INFINITY;
		return (0);
	}
	return (1);
}

static t_vector		calc_cy_normal(double x2[2], t_vector o, t_vector d, t_figure *lst)
{
	double	dist;
	double	x;

	if ((lst->figure.cy.dist1 >= 0 && lst->figure.cy.dist1 <= lst->figure.cy.height && x2[0] > EPSILON)
	 && (lst->figure.cy.dist2 >= 0 && lst->figure.cy.dist2 <= lst->figure.cy.height && x2[1] > EPSILON))
	{
		dist = lst->figure.cy.dist2;
		x = x2[1];
		if (x2[0] < x2[1])
		{
			dist = lst->figure.cy.dist1;
			x = x2[0];
		}
	}
	else if (lst->figure.cy.dist1 >= 0 && lst->figure.cy.dist1 <= lst->figure.cy.height
														&& x2[0] > EPSILON)
	{
		dist = lst->figure.cy.dist1;
		x = x2[0];
	}
	else
	{
		dist = lst->figure.cy.dist2;
		x = x2[1];
	}
	x2[0] = x;
	return (vector_norm(vector_sub(vector_sub(vector_mlt(x, d),
			vector_mlt(dist, lst->figure.cy.nv)), vector_sub(lst->figure.cy.center, o))));
}

static double	cy_intersection(t_vector o, t_vector d, t_vector *normal, t_figure *lst)
{
	double	x2[2];

	if (solve_cylinder(x2, o, d, lst) == 0)
		return (INFINITY);
	lst->figure.cy.dist1 = vector_dot(lst->figure.cy.nv, vector_sub(vector_mlt(x2[0], d),
												vector_sub(lst->figure.cy.center, o)));
	lst->figure.cy.dist2 = vector_dot(lst->figure.cy.nv, vector_sub(vector_mlt(x2[1], d),
												vector_sub(lst->figure.cy.center, o)));
	if (!((lst->figure.cy.dist1 >= 0 && lst->figure.cy.dist1 <= lst->figure.cy.height
					&& x2[0] > EPSILON) || (lst->figure.cy.dist2 >= 0
					&& lst->figure.cy.dist2 <= lst->figure.cy.height && x2[0] > EPSILON)))
		return (INFINITY);
	*normal = calc_cy_normal(x2, o, d, lst);
	return (x2[0]);
}

static double	caps_intersection(t_vector o, t_vector d, t_figure *lst)
{
	double	id1;
	double	id2;
	t_vector	ip1;
	t_vector	ip2;
	t_vector	c2; //center2

	c2 = vector_sum(lst->figure.cy.center, vector_mlt(lst->figure.cy.height, lst->figure.cy.nv));
	id1 = solve_plane(o, d, lst->figure.cy.center, lst->figure.cy.nv);
	id2 = solve_plane(o, d, c2, lst->figure.cy.nv);
	if (id1 < INFINITY || id2 < INFINITY)
	{
		ip1 = vector_sum(o, vector_mlt(id1, d));
		ip2 = vector_sum(o, vector_mlt(id2, d));
		if ((id1 < INFINITY && distance(ip1, lst->figure.cy.center) <= lst->figure.cy.radius)
				&& (id2 < INFINITY && distance(ip2, c2) <= lst->figure.cy.radius))
			return (id1 < id2 ? id1 : id2);
		else if (id1 < INFINITY
						&& distance(ip1, lst->figure.cy.center) <= lst->figure.cy.radius)
			return (id1);
		else if (id2 < INFINITY && distance(ip2, c2) <= lst->figure.cy.radius)
			return (id2);
		return (INFINITY);
	}
	return (INFINITY);
}

double			cylinder_intersection(t_vector o, t_vector d, t_figure *lst)
{
	double		cylinder_inter;
	double		caps_inter;
	t_vector	cy_normal;

	cylinder_inter = cy_intersection(o, d, &cy_normal, lst);
	caps_inter = caps_intersection(o, d, lst);
	if (cylinder_inter < INFINITY || caps_inter < INFINITY)
	{
		if (cylinder_inter < caps_inter)
		{
			lst->normal = cy_normal;
			return (cylinder_inter);
		}
		lst->normal = lst->figure.cy.nv;
		return (caps_inter);
	}
	return (INFINITY);
}
