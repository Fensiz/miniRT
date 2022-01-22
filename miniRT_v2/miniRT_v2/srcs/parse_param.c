/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_param.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 14:10:05 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 14:10:06 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_ambient_light(t_scene *scene, char **str)
{
	if (scene->amb_count != 0)
		ft_error(SCENE, "Extra ambient light");
	(scene->amb_count)++;
	(*str)++;
	skip_spaces(str);
	scene->ambient_light = ft_atof(str);
	check_value(scene->ambient_light, 0, 1, "ambient lightning");
	scene->ambient_light_color = parse_color(str);
}

void	parse_camera(t_mlx *mlx, t_scene *scene, char **str)
{
	if (scene->cam_count != 0)
		ft_error(SCENE, "Extra camera");
	(scene->cam_count)++;
	(*str)++;
	mlx->camera = memalloc(sizeof(t_camera));
	mlx->camera->next = NULL;
	skip_spaces(str);
	mlx->camera->origin = parse_vector(str);
	mlx->camera->direction = vector_norm(parse_vector(str));
	mlx->camera->fov = ft_atoi(str);
	check_value(mlx->camera->fov, 0, 180, "camera");
}

void	parse_light(t_scene **scene, char **str)
{
	t_light	*new;
	t_light	*curr;

	if ((*scene)->light_count != 0 && !BNS)
		ft_error(SCENE, "Extra light");
	((*scene)->light_count)++;
	(*str)++;
	curr = (*scene)->light;
	new = memalloc(sizeof(t_light));
	new->next = NULL;
	if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	else
		(*scene)->light = new;
	skip_spaces(str);
	new->origin = parse_vector(str);
	new->brightness = ft_atof(str);
	check_value(new->brightness, 0, 1, "light");
	new->color = parse_color(str);
}
