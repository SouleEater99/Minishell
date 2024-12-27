/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:47:59 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 12:48:00 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	if (!str)
		return (NULL);
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

int	ft_check_export_plus(char *str, int i)
{
	if (str[i] == '+')
	{
		if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			return (2);
		else
			return (0);
	}
	return (1);
}
