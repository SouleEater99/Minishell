/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:13:39 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 10:14:11 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	quotes(char *line, int index)
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

static void	process_quote(t_quote_process *qp)
{
	if (qp->str[qp->i] == '\'' && qp->dobqot == 1)
		qp->sinqot *= (-1);
	if (qp->str[qp->i] == '\"' && qp->sinqot == 1)
		qp->dobqot *= (-1);
	if (qp->str[qp->i] == '\'' && qp->dobqot == 1)
		qp->i++;
	else if (qp->str[qp->i] == '\"' && qp->sinqot == 1)
		qp->i++;
	else
	{
		qp->newstr = apend_char_str(qp->newstr, qp->str[qp->i]);
		qp->i++;
	}
}

char	*rm_quotes(char *str)
{
	t_quote_process	qp;

	if (!str)
		return (NULL);
	qp.i = 0;
	qp.sinqot = 1;
	qp.dobqot = 1;
	qp.newstr = NULL;
	qp.str = str;
	while (qp.str[qp.i])
	{
		process_quote(&qp);
	}
	if (qp.newstr == NULL)
		qp.newstr = apend_char_str(qp.newstr, '\0');
	free(str);
	return (qp.newstr);
}

void	del_q(t_parser *parse)
{
	t_parser	*cur;
	int			i;

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
