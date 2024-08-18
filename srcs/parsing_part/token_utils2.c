/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:05:27 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/15 11:16:43 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_sep(char *line, int i)
{
    if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
        return 0;
    else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
        return 1;
    else
        return 0;
}

int		is_types(t_token *token, char *types)
{
	if (ft_strchr(types, ' ') && is_type(token, EMPTY))
		return (1);
	else if (ft_strchr(types, 'X') && is_type(token, CMD))
		return (1);
	else if (ft_strchr(types, 'x') && is_type(token, ARG))
		return (1);
	else if (ft_strchr(types, 'T') && is_type(token, TRUNC))
		return (1);
	else if (ft_strchr(types, 'A') && is_type(token, APPEND))
		return (1);
	else if (ft_strchr(types, 'I') && is_type(token, INF))
		return (1);
	else if (ft_strchr(types, 'P') && is_type(token, PIPE))
		return (1);
	else if (ft_strchr(types, 'H') && is_type(token, HEREDOC))
		return (1);
	return (0);
}

void	line_args(t_tools *mini)
{
	t_token	*token;
	t_token	*prev;

	token = mini->cmd;
	while (token)
	{
		prev = prev_sep(token, 0);
		if (is_type(token, ARG) && is_types(prev, "TAIH"))
		{
			while (is_last_valid_arg(prev) == 0)
				prev = prev->prev;
			token->prev->next = token->next;
			if (token->next)
				token->next->prev = token->prev;
			token->prev = prev;
			token->next = (prev) ? prev->next : mini->cmd;
			prev = (prev) ? prev : token;
			prev->next->prev = token;
			prev->next = (mini->cmd->prev) ? prev->next : token;
			mini->cmd = (mini->cmd->prev) ? mini->cmd->prev : mini->cmd;
		}
		token = token->next;
	}
}

