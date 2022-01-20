/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:30:57 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 17:30:58 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_atoi(char **str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	skip_spaces(str);
	if (**str == '-' && *(*str)++)
		sign = -1;
	while (ft_isdigit(**str))
		res = res * 10 + *(*str)++ - '0';
	skip_spaces(str);
	return (res * sign);
}

double	ft_atof(char **str)
{
	int		int_res;
	double	res;
	int		sign;

	int_res = 0;
	sign = 1;
	skip_spaces(str);
	if (**str == '-' && (*str)++)
		sign = -1;
	while (ft_isdigit(**str))
		int_res = int_res * 10 + *(*str)++ - '0';
	if (**str == '.')
		(*str)++;
	res = 0.0;
	while (ft_isdigit(**str))
		res = res * 10 + *(*str)++ - '0';
	while (res >= 1)
		res /= 10;
	res += int_res;
	skip_spaces(str);
	return (res * sign);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;

	p = b;
	while (len--)
		*p++ = (unsigned char)c;
	return (b);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (dlen >= dstsize)
		return (dstsize + slen);
	dst += dlen;
	i = dlen + 1;
	while (*src && i++ < dstsize)
		*dst++ = *src++;
	*dst = 0;
	return (dlen + slen);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize && src && dst)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	return (ft_strlen(src));
}
