/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 14:38:07 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 14:38:09 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <mlx.h>

void	init_mlx(t_mlx *mlx, t_scene *scene)
{
	t_camera	*curr;

	mlx->mlx = mlx_init();
	scene->width = 800;
	scene->height = 600;
	curr = mlx->camera;
	mlx->begin = mlx->camera;
	while (curr)
	{
		curr->image = mlx_new_image(mlx->mlx, scene->width, scene->height);
		curr->img_addr = mlx_get_data_addr(mlx->camera->image,
				&mlx->camera->bits_per_pixel,
				&curr->line_length, &curr->endian);
		curr = curr->next;
	}
}

void	my_put_pixel(t_mlx *mlx, int x, int y, int color)
{
	char	*dst;

	dst = mlx->camera->img_addr + (y * mlx->camera->line_length
			+ x * (mlx->camera->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
