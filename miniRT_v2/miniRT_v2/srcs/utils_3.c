/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:32:11 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:32:12 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	*memalloc(unsigned int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		ft_fatal(MALLOC);
	return (ptr);
}

int	ft_isspace(char c)
{
	if ((c >= 11 && c <= 13) || c == ' ' || c == 9)
		return (1);
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	skip_spaces(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}
