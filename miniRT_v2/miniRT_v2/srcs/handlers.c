/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:27:51 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:27:52 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <mlx.h>

int	key_handler(int keycode, void *mlx_arr)
{
	if (keycode == 53)
	{
		mlx_destroy_window(((t_mlx *)mlx_arr)->mlx,
			((t_mlx *)mlx_arr)->window);
		free(((t_mlx *)mlx_arr)->mlx);
		exit(0);
	}
	return (0);
}

int	red_cross_handler(void *mlx_arr)
{
	mlx_destroy_window(((t_mlx *)mlx_arr)->mlx, ((t_mlx *)mlx_arr)->window);
	free(((t_mlx *)mlx_arr)->mlx);
	exit(0);
}
