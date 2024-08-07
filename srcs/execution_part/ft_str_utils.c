#include "../../include/minishell.h"


size_t ft_strlen_delimiter(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '	')
		i++;
	return (i);
}

char *ft_strjoin_path(char const *s1, char const *s2, char c)
{
	size_t i;
	size_t s1_len;
	char *d;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1) + 1;
	d = (char *)malloc(s1_len + ft_strlen_delimiter(s2) + 1);
	if (d == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		d[i] = s1[i];
	d[i] = c;
	i = 0;
	while (s2[i] && s2[i] != ' ' && s2[i] != '	')
	{
		d[s1_len + i] = s2[i];
		i++;
	}
	d[s1_len + i] = '\0';
	return (d);
}

int ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
