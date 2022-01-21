/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:33:50 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:33:51 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_map(char *str, t_map *map)
{
	int	i;

	map->size = ft_atoi(&str);
	if (*str != '\n')
		ft_error(SCENE, "map parse error");
	map->map = (int *)memalloc(map->size * map->size * sizeof(int));
	i = 0;
	while (*str && i < map->size * map->size)
	{
		if (!(*str >= '0' && *str <= '9') && *str != ' ' && *str != '-')
			str++;
		else
		{
			map->map[i] = ft_atoi(&str);
			i++;
		}
	}
}

t_map	*alloc_map(void)
{
	t_map	*map;

	map = (t_map *)memalloc(sizeof(t_map));
	map->size = 0;
	map->map = NULL;
	map->next = NULL;
	return (map);
}

void	load_one_map(t_map *map, const char *fname)
{
	char	*str;
	int		fd;

	fd = open(fname, O_RDONLY);
	if (fd == -1)
		ft_fatal(FD);
	str = file_to_str(fd);
	parse_map(str, map);
	close(fd);
	free(str);
}

void	load_map(t_scene *scene)
{
	scene->map = alloc_map();
	load_one_map(scene->map, "./texture/map_1.bump");
	scene->map->next = alloc_map();
	load_one_map(scene->map->next, "./texture/map_2.bump");
	scene->map->next->next = alloc_map();
	load_one_map(scene->map->next->next, "./texture/map_3.bump");
}

/* "/Users/simon/miniRT_git/miniRT_v2/miniRT_v2/texture/map_2.bump"); */
