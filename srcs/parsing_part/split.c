/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 08:58:19 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 11:37:44 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*d;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	if (start >= strlen(s))
		return (strdup(""));
	if (start + len > strlen(s))
		len = strlen(s) - start;
	d = malloc(len + 1);
	if (!d)
		return (NULL);
	while (s[i])
	{
		if (i >= start && j < len)
			d[j++] = s[i];
		i++;
	}
	d[j] = '\0';
	return (d);
}

static void	process_character(const char *str, int i, int *in_word, int *count)
{
	if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
	{
		if (*in_word)
		{
			*in_word = 0;
			(*count)++;
		}
	}
	else
	{
		if (!*in_word)
			*in_word = 1;
	}
}

static int	count_words(const char *str)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		process_character(str, i, &in_word, &count);
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

static void	process_quotes(char *str, t_vars *vars)
{
	while (str[vars->j] && ((str[vars->j] != 32 && str[vars->j] != '\t'
				&& str[vars->j] != '\n') || vars->dblqt == -1 || vars->sinqt
			== -1))
	{
		if (str[vars->j] == '\'' && vars->dblqt == 1)
			vars->sinqt *= -1;
		if (str[vars->j] == '\"' && vars->sinqt == 1)
			vars->dblqt *= -1;
		vars->j++;
	}
}

char	**ft_split_it(char *str)
{
	t_vars	vars;
	char	**s;

	vars.dblqt = 1;
	vars.sinqt = 1;
	vars.i = 0;
	vars.j = 0;
	vars.n = 0;
	s = ft_calloc(sizeof(char *), count_words(str) + 1);
	if (s == NULL)
		return (NULL);
	while (str && str[vars.i])
	{
		while ((str[vars.i] == 32 || str[vars.i] == '\t' || str[vars.i] == '\n')
			&& vars.dblqt == 1)
			vars.i++;
		if (str[vars.i] == '\0')
			break ;
		vars.j = vars.i;
		process_quotes(str, &vars);
		s[vars.n++] = ft_substr(str, vars.i, vars.j - vars.i);
		vars.i = vars.j;
	}
	return (s);
}
