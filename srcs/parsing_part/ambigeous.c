/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambigeous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:52:11 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 11:53:00 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	amb_count_args(char **tab)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!tab)
		return (-1);
	while (tab[i])
	{
		count++;
		i++;
	}
	return (count);
}

void	ambigeous_file(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->args && cur->args[0])
		{
			if (cur && cur->type == FILENAME && amb_count_args(cur->args) > 1)
			{
				if (cur->input)
					free(cur->input);
				cur->input = NULL;
			}
		}
		else if (amb_count_args(cur->args) == 0 && cur->type == FILENAME)
		{
			if (cur->input)
				free(cur->input);
			cur->input = NULL;
		}
		cur = cur->next;
	}
}
