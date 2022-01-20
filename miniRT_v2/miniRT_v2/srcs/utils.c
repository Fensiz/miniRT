
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

void		*memalloc(unsigned int size)
{
	void *ptr;

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
void		skip_spaces(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}

int	ft_atoi(char **str)
{
	int res;
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
