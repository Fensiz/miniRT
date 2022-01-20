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
	(*str)++;
	skip_spaces(str);
	scene->ambient_light = ft_atof(str);
	check_value(scene->ambient_light, 0, 1, "ambient lightning");
	scene->ambient_light_color = parse_color(str);
}

void	parse_camera(t_mlx *mlx, t_scene *scene, char **str)
{
	t_camera	*new;
	t_camera	*curr;
	int			prev_cam_id;

	(*str)++;
	prev_cam_id = 0;
	curr = mlx->camera;
	new = memalloc(sizeof(t_camera));
	new->next = NULL;
	if (curr)
	{
		while (curr->next)
			curr = curr->next;
		prev_cam_id = curr->cam_id;
		curr->next = new;
	}
	else
		mlx->camera = new;
	skip_spaces(str);
	new->cam_id = prev_cam_id + 1;
	scene->cam_count = new->cam_id;
	new->origin = parse_vector(str);
	new->direction = vector_norm(parse_vector(str));
	new->fov = ft_atoi(str);
	check_value(new->fov, 0, 180, "camera");
}

void	parse_light(t_scene **scene, char **str)
{
	t_light	*new;
	t_light	*curr;

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
