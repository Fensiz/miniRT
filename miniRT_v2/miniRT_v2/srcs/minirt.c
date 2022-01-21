/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:36:21 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:36:23 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	trace_ray(t_vector_2p ray, int depth, t_figure *lst, t_scene *scene)
{
	t_data		data;

	data.inter = get_inter(&data.closest_figure, ray, lst, scene);
	if (data.closest_figure.type == -1)
	{
		free(data.inter);
		return (scene->background);
	}
	apply_texture(&data.closest_figure, data.inter, scene);
	data.inter->figure = &data.closest_figure;
	calc_light(ray, data.inter, *scene, lst);
	data.r = data.closest_figure.reflection;
	data.inter->reflection_color = 0;
	if (data.r > 0 && depth > 0)
	{
		data.ray = vector_2p_set(data.inter->point,
				reflect_ray(vector_mlt(-1, ray.direction), data.inter->normal));
		if (vector_dot(data.ray.direction, data.inter->old_normal) >= 0)
			data.inter->reflection_color
				= trace_ray(data.ray, depth - 1, lst, scene);
	}
	data.ret_color = color_sum(color_mlt(data.inter->color, 1 - data.r),
			color_mlt(data.inter->reflection_color, data.r));
	free(data.inter);
	return (data.ret_color);
}

int	main(int argc, const char **argv)
{
	t_mlx		mlx;
	t_scene		scene;
	t_figure	*figure;

	if (argc != 2)
		ft_usage_message(argv[0]);
	parse_scene(&mlx, &scene, &figure, argv);
	init_mlx(&mlx, &scene);
	load_map(&scene);
	scene.background = 0;
	render_scene(&scene, figure, &mlx, mlx.camera);
	mlx.window = mlx_new_window(mlx.mlx, scene.width, scene.height, "miniRT");
	mlx_put_image_to_window(mlx.mlx, mlx.window, mlx.camera->image, 0, 0);
	mlx_hook(mlx.window, 2, 1L << 0, key_handler, &mlx);
	mlx_hook(mlx.window, 17, 0L, red_cross_handler, &mlx);
	free_bump(&scene);
	free(mlx.camera->img_addr);
	free(mlx.camera->image);
	free(mlx.camera);
	free_figures(figure);
	free_light(scene.light);
	mlx_loop(mlx.mlx);
	return (0);
}
