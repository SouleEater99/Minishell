/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:13:39 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/06 17:38:57 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		quotes(char *line, int index)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i] && i != index)
	{
		if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 0;
		else if (open == 2 && line[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}

static void process_quote(char *str, int *i, int *sinqot, int *dobqot, char **newstr)
{
    if (str[*i] == '\'' && *dobqot == 1)
        *sinqot *= (-1);
    if (str[*i] == '\"' && *sinqot == 1)
        *dobqot *= (-1);
    if (str[*i] == '\'' && *dobqot == 1)
        (*i)++;
    else if (str[*i] == '\"' && *sinqot == 1)
        (*i)++;
    else
    {
        *newstr = apend_char_str(*newstr, str[*i]);
        (*i)++;
    }
}

char	*rm_quotes(char *str)
{
	int		i;
	int		sinqot;
	int		dobqot;
	char	*newstr;

	i = 0;
	sinqot = 1;
	dobqot = 1;
	newstr = NULL;
	if (!str)
		return (NULL);
	while (str[i])
	{
		process_quote(str, &i, &sinqot, &dobqot, &newstr);
	}
	if (newstr == NULL)
		newstr = apend_char_str(newstr, '\0');
	free(str);
	return (newstr);
}

void	del_q(t_parser *parse)
{
	t_parser	*cur;
	int	i;

	cur = parse;

	expand_flag(parse);
	while (cur)
	{
		i = 0;
		cur->str = rm_quotes(cur->str);
		if (cur->args)
		{
			while (cur->args[i])
			{
				cur->args[i] = rm_quotes(cur->args[i]);
				i++;
			}
			
		}
		cur = cur->next;
	}
	
}
