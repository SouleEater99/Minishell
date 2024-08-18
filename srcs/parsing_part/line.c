/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:01:26 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/17 18:32:19 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	alloc(char *line, int *start)
{
	int		count;
	int		i;
	char	c;

	count = 0;
	i = 0;
	c = ' ';
	while (line[*start + i] && (line[*start + i] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*start + i] == '\'' || line[*start + i] == '\"'))
		{
			c = line[*start + i++];
			count++;
		}
		else if (c != ' ' && line[*start + i] == c)
		{
			count++;
			c = ' ';
			i++;
		}
		else
			i++;
	}
	return (i + count + 1);
}

char	*allocate_line(char *line)
{
	char	*new;
	int		count;
	int		i;
	int		dollar_count;

	count = 0;
	dollar_count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		if (line[i] == '$')
			dollar_count++;
		i++;
	}
	new = malloc(sizeof(char) * (i + 2 * count + dollar_count + 1));
	if (!new)
		return (NULL);
	return (new);
}

// static void	ext_format_line(int	i, int j, char *line, char *new)
// {
// }

char	*formate_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = allocate_line(line);
	while (new && line[i])
	{
		if (quotes(line, i) != 2 && line[i] == '$')
		{
			// Handle consecutive dollar signs
			while (line[i] == '$')
			{
				new[j++] = line[i++];
			}
		}
		else if (quotes(line, i) == 0 && line[i] == '|')
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			new[j++] = ' ';
		}
		else if (quotes(line, i) == 0 && is_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '<')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
	ft_memdel(line);
	return (new);
}
