#include "minirt.h"
#include <stdio.h>

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
		//write(1,"XK\n",3);
		if (!(*str >= '0' && *str <= '9') && *str != ' ' && *str != '-')
			str++;
		else
			map->map[i++] = ft_atoi(&str);
	}
}

void	load_map(t_scene *scene)
{
	int		fd;
	char	*str;
	//write(1,"OK\n",3);
	fd = open(//"./texture/map_1.bump"
			  "/Users/simon/miniRT_git/miniRT_v2/miniRT_v2/texture/map_1.bump", O_RDONLY);
	if (fd == -1)
		ft_fatal(FD);
	str = file_to_str(fd);
	scene->map = (t_map *)memalloc(sizeof(t_map));
	//scene->map->next = NULL;
	parse_map(str, scene->map);
	close(fd);
	free(str);
//	fd = open("./texture/map_2.bump", O_RDONLY);
//	if (fd == -1)
//		ft_fatal(FD);
//	str = file_to_str(fd);
//	scene->map->next = memalloc(sizeof(t_map *));
//	scene->map->next->next = NULL;
//	parse_map(str, scene->map->next);
//	close(fd);
//	free(str);
//	fd = open("./texture/map_3.bump", O_RDONLY);
//	if (fd == -1)
//		ft_fatal(FD);
//	str = file_to_str(fd);
//	scene->map->next->next = memalloc(sizeof(t_map *));
//	scene->map->next->next->next = NULL;
//	parse_map(str, scene->map->next->next);
//	close(fd);
//	write(1,"OK\n",3);
//	free(str);
}
