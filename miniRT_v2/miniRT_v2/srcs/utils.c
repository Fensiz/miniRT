/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:32:26 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:32:28 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_fatal(int i)
{
	if (i == MALLOC)
		write(2, "Fatal error: malloc failed.\n", 28);
	if (i == FD)
		write(2, "Cannot open file.\n", 18);
	exit(i);
}

size_t	ft_strlen(const char *s)
{
	size_t	counter;

	counter = 0;
	while (s[counter])
		counter++;
	return (counter);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*new;
	size_t	i;

	len_s1 = 0;
	len_s2 = 0;
	if (s1)
		len_s1 = ft_strlen(s1);
	if (s2)
		len_s2 = ft_strlen(s2);
	new = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!new)
		ft_fatal(MALLOC);
	new[len_s1 + len_s2] = 0;
	i = 0;
	while (i < len_s1)
		*new++ = s1[i++];
	i = 0;
	while (i < len_s2)
		*new++ = s2[i++];
	return (new - len_s1 - len_s2);
}
