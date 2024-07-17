#include "../../include/minishell.h"

// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] != c)
// 	{
// 		if (s[i] == '\0')
// 			return (NULL);
// 		i++;
// 	}
// 	return ((char *)s + i);
// }

// int ft_strcmp(char *s1, char *s2)
// {
//     if (!s1 || !s2)
//         return (-1);
//     while (*s1 && *s2)
//     {
//         if (*s1 != *s2)
//             return (*s1 - *s2);
//         s1++;
//         s2++;
//     }
//     return (*s1 - *s2);
// }

void	ft_putstr_fd(char *s, int fd)
{
	if (fd >= 0 && s && *s)
		while (*s)
			write(fd, s++, 1);
}

char	*ft_strdup(const char *str)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (str[i])
		i++;
	dup = (char *)malloc(i + 1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_memset(void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*s;

	i = 0;
	s = (unsigned char *)str;
	while (i < n)
		s[i++] = (unsigned char)c;
	return (str);
}