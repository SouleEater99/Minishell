/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:27:49 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 12:33:53 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	heredoc_quotes(char *line)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i])
	{
		if (i > 0 && line[i - 1] == '\\')
			i++;
		else if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 4;
		else if (open == 2 && line[i] == '\'')
			open = 6;
		i++;
	}
	return (open);
}

void	expand_flag(t_parser *parser)
{
	t_parser	*cur;
	int			quote_status;

	cur = parser;
	while (cur)
	{
		if (cur && cur->type == HEREDOC)
		{
			quote_status = heredoc_quotes(cur->args[0]);
			if (quote_status == 4 || quote_status == 6)
			{
				cur->index = 1;
			}
		}
		cur = cur->next;
	}
}
