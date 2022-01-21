/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:43:06 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/21 16:43:07 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
