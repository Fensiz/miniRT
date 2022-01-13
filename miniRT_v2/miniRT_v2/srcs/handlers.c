
#include "minirt.h"
#include <mlx.h>

int	key_handler(int keycode, void *mlx_arr)
{
	if (keycode == 53)
	{
		mlx_destroy_window (((t_mlx *)mlx_arr)->mlx, ((t_mlx *)mlx_arr)->window);
		exit (0);
	}
	return (0);
}

int	red_cross_handler(void *mlx_arr)
{
	mlx_destroy_window (((t_mlx *)mlx_arr)->mlx, ((t_mlx *)mlx_arr)->window);
	exit (0);
}
