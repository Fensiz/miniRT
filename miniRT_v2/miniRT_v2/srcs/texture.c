#include "minirt.h"

static int	checkerboard(t_inter *inter)
{
	t_vector	coords;
	t_vector	val;
	int		party_mix;

	coords.x = abs((int)floor(inter->point.x));
	coords.y = abs((int)floor(inter->point.y));
	coords.z = abs((int)floor(inter->point.z));
	val.x = (int)coords.x % 2;
	val.y = (int)coords.y % 2;
	val.z = (int)coords.z % 2;
	party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
	return (party_mix ? BLACK : WHITE);
}

void	apply_texture(int texture, t_inter *inter)
{
	if (texture == 1)
		inter->color = checkerboard(inter);
}
