/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 14:10:11 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 14:10:12 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	check_value(double value, double min, double max, char *name)
{
	char	error[100];

	if (value < min || value > max)
	{
		ft_strlcpy(error, name, 81);
		ft_strlcat(error, " value out of range", 100);
		ft_error(SCENE, error);
	}
}

int	parse_color(char **str)
{
	int	red;
	int	green;
	int	blue;

	red = ft_atoi(str);
	check_value(red, 0, 255, "color");
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	green = ft_atoi(str);
	check_value(green, 0, 255, "color");
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	blue = ft_atoi(str);
	check_value(blue, 0, 255, "color");
	return ((red << 16) + (green << 8) + blue);
}

t_vector	parse_vector(char **str)
{
	t_vector	v;

	v.x = ft_atof(str);
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	v.y = ft_atof(str);
	if (**str != ',')
		ft_error(SCENE, " parse error: expected comma");
	(*str)++;
	v.z = ft_atof(str);
	return (v);
}

static t_figure	*ft_newfigure(void)
{
	t_figure	*new;

	new = memalloc(sizeof(t_figure));
	new->next = NULL;
	return (new);
}

t_figure	*ft_addback_figure(t_figure **lst)
{
	t_figure	*curr;

	curr = *lst;
	if (!curr)
	{
		*lst = ft_newfigure();
		curr = *lst;
	}
	else
	{
		while (curr->next)
			curr = curr->next;
		curr->next = ft_newfigure();
		curr = curr->next;
	}
	return (curr);
}
