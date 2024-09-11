/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:58:13 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 21:54:10 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void	free_token_list(t_token *token)
{
	t_token	*next;
	int		i;

	while (token)
	{
		next = token->next;
		free(token->input);
		if (token->args)
		{
			i = 0;
			while (token->args[i])
			{
				free(token->args[i]);
				i++;
			}
			free(token->args);
		}
		free(token);
		token = next;
	}
}

void	free_parser_list(t_parser *pars)
{
	t_parser	*tmp;

	while (pars)
	{
		tmp = pars->next;
		free(pars);
		pars = tmp;
	}
}
