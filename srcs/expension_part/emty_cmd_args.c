/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emty_cmd_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:30:06 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 12:30:43 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_token(t_token **token, t_token **cur, t_token **prev,
		t_token *next)
{
	if (((*cur)->type == CMD || (*cur)->type == ARG)
		&& (*cur)->input[0] == '\0')
	{
		if (*prev)
			(*prev)->next = next;
		else
			*token = next;
		if (next)
			next->prev = *prev;
		free((*cur)->input);
		free(*cur);
		*cur = next;
	}
	else
	{
		*prev = *cur;
		*cur = next;
	}
}

void	ignore_empty_cmd_arg(t_token **token)
{
	t_token	*cur;
	t_token	*next;
	t_token	*prev;

	if (!token || !*token)
		return ;
	cur = *token;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		process_token(token, &cur, &prev, next);
	}
	get_tokens_type(*token, 0);
}
