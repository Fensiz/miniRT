/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 14:13:53 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 14:13:54 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse(t_mlx *mlx, t_scene *scene, t_figure **figure, char **s)
{
	if (**s == 'A')
		parse_ambient_light(scene, s);
	else if (**s == 'C')
		parse_camera(mlx, scene, s);
	else if (**s == 'c' && *(*s + 1) == 'y')
		parse_cylinder(figure, s);
	else if (**s == 'c' && *(*s + 1) == 'o')
		parse_cone(figure, s);
	else if (**s == 'L')
		parse_light(&scene, s);
	else if (**s == 's' && *(*s + 1) == 'p')
		parse_sphere(figure, s);
	else if (**s == 'p' && *(*s + 1) == 'l')
		parse_plane(figure, s);
	else
		ft_error(SCENE, " parse error: undefined object in the input file");
}

void	parse_elems(t_mlx *mlx, t_scene *scene, t_figure **figure, char *str)
{
	while (*str)
	{
		while (ft_isspace(*str) || *str == '\n' || *str == '#')
		{
			if (ft_isspace(*str))
				skip_spaces(&str);
			else if (*str == '\n')
				str ++;
			else
			{
				while (*str && *str != '\n')
					str ++;
			}
		}
		if (!*str)
			break ;
		parse(mlx, scene, figure, &str);
	}
	if (mlx->camera == NULL)
		ft_error(SCENE, " parse error: no cam");
}

static int	is_right_format(const char *fname)
{
	int	len;

	len = (int)ft_strlen(fname);
	if (len <= 3)
		return (0);
	if (!(fname[len - 1] == 't' && fname[len - 2] == 'r'
			&& fname[len - 3] == '.'))
		return (0);
	return (1);
}

void	parse_scene(t_mlx *mlx, t_scene *scene, t_figure **figure,
					const char **argv)
{
	char		*str;
	int			fd;

	*figure = NULL;
	scene->cam_count = 0;
	scene->amb_count = 0;
	scene->light_count = 0;
	scene->light = NULL;
	mlx->camera = NULL;
	fd = open(argv[1], O_RDONLY);
	if (!is_right_format(argv[1]))
		ft_error(FILE_FORMAT_ERR, "");
	if (fd == -1)
		ft_fatal(FD);
	str = file_to_str(fd);
	parse_elems(mlx, scene, figure, str);
	free(str);
}
