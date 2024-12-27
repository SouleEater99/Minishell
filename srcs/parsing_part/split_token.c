/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:37:25 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 11:37:50 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	split_token_input(t_token *token)
{
	t_token	*cur;

	if (!token)
		return ;
	cur = token;
	while (cur)
	{
		if (is_types(cur, "XGF"))
		{
			cur->args = ft_split_it(cur->input);
		}
		else
			cur->args = NULL;
		cur = cur->next;
	}
}
