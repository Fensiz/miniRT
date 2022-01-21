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

void	ft_usage_message(const char *program_name)
{
	write(1, "Usage: ", 7);
	write(1, program_name, ft_strlen(program_name));
	write(1, " <scene.rt>\n", 11);
	exit(EXIT_FAILURE);
}

void	ft_error(int code, char *error_text)
{
	if (code == SCENE)
		write(2, "Scene error: ", 13);
	else if (code == FATAL)
		write(2, "Fatal error: ", 13);
//	else if (code == FIG_TYPE)
//		write(2, "Figure error: ", 14);
	else if (code == FILE_FORMAT_ERR)
		write(2, "Wrong file format", 17);
	write(2, error_text, ft_strlen(error_text));
	write(2, "\n", 1);
	exit(code);
}

char	*file_to_str(int fd)
{
	char	buffer[BUFF_SIZE + 1];
	char	*str;
	char	*tmp;
	size_t	r_size;

	str = NULL;
	r_size = read(fd, buffer, BUFF_SIZE);
	while (r_size)
	{
		tmp = str;
		buffer[r_size] = '\0';
		str = ft_strjoin(str, buffer);
		free(tmp);
		r_size = read(fd, buffer, BUFF_SIZE);
	}
	return (str);
}

t_inter	*get_inter(t_figure *closest_figure, t_vector_2p ray, t_figure *lst,
	t_scene *scene)
{
	double		closest_intersection;
	t_inter		*inter;

	inter = (t_inter *)memalloc(sizeof(t_inter));
	closest_intersection = INFINITY;
	closest_figure->type = -1;
	get_closest_inter(ray, lst, closest_figure, &closest_intersection);
	inter->point = vector_sum(ray.origin, vector_mlt(closest_intersection,
				ray.direction));
	calc_normal(inter->point, ray.direction, &inter->normal, closest_figure);
	inter->color = scene->background;
	inter->old_normal = inter->normal;
	if (closest_figure->type != -1)
	{
		inter->color = closest_figure->color;
	}
	return (inter);
}

int	trace_ray(t_vector_2p ray, int depth, t_figure *lst, t_scene *scene)
{
	t_figure	closest_figure;
	t_inter		*inter;
	double		r;
	t_vector_2p	new_ray;
	int			ret_color;

	inter = get_inter(&closest_figure, ray, lst, scene);
	if (closest_figure.type == -1)
	{
		free(inter);
		return (scene->background);
	}
	apply_texture(&closest_figure, inter, scene);
	inter->figure = &closest_figure;
	calc_light(ray, inter, *scene, lst);
	r = closest_figure.reflection;
	inter->reflection_color = 0;
	if (r > 0 && depth > 0)
	{
		new_ray.origin = inter->point;
		new_ray.direction = reflect_ray(vector_mlt(-1, ray.direction), inter->normal);
		if (vector_dot(vector_mlt(1, new_ray.direction), inter->old_normal) >= 0)
			inter->reflection_color = trace_ray(new_ray, depth - 1, lst, scene);
	}
	ret_color = color_sum(color_mlt(inter->color, 1 - r),
			color_mlt(inter->reflection_color, r));
	free(inter);
	return (ret_color);
}

void	free_bump(t_scene *s)
{
	free(s->map->map);
	free(s->map->next->map);
	free(s->map->next->next->map);
	free(s->map->next->next);
	free(s->map->next);
	free(s->map);
}

void	free_figures(t_figure *f)
{
	t_figure	*tmp;

	if (f)
	{
		while (f)
		{
			tmp = f->next;
			free(f);
			f = tmp;
		}
	}
}

void	free_light(t_light	*l)
{
	t_light	*tmp;

	if (l)
	{
		while (l)
		{
			tmp = l->next;
			free(l);
			l = tmp;
		}
	}
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
