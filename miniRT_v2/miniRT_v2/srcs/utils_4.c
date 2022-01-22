/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:41:53 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/21 16:41:54 by bgreenbl         ###   ########.fr       */
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
	else if (code == FILE_FORMAT_ERR)
		write(2, "Wrong file format", 17);
	write(2, error_text, ft_strlen(error_text));
	write(2, "\n", 1);
	exit(code);
}

char	*file_to_str(int fd)
{
	char	buffer[BUFF_SIZE + 2];
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
	tmp = str;
	str = ft_strjoin(str, "");
	free(tmp);
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
