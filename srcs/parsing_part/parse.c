/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:09:10 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/15 12:28:05 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**line_tab(t_token *start)
{
	t_token	*token;
	char	**tab;
	int		i;

	if (!start)
		return (NULL);
	token = start->next;
	i = 2;
	while (token && token->type < TRUNC)
	{
		token = token->next;
		i++;
	}
	if (!(tab = malloc(sizeof(char *) * i)))
		return (NULL);
	token = start->next;
	tab[0] = ft_strdup(start->input);
	i = 1;
	while (token && token->type < TRUNC)
	{
		tab[i++] = ft_strdup(token->input);
		token = token->next;
	}
	tab[i] = NULL;
	return (tab);
}

t_parser *fill_parse_struct(t_tools *tools)
{
    t_parser *pars = NULL;
    t_parser *current_parser = NULL;
    t_token *token = tools->cmd;
	
	if (token && token->type == ARG)
		token->type = CMD;
    while (token)
    {
		if (!pars)
        {
            pars = malloc(sizeof(t_parser));
            current_parser = pars;
        }
        else
        {
            current_parser->next = malloc(sizeof(t_parser));
            current_parser = current_parser->next;
        }
        // Set the values for the current parser node
		if (token && token->type != ARG)
		{
			current_parser->str = ft_strdup(token->input);
			current_parser->type = token->type;
			if (token->type != PIPE)
				current_parser->args = line_tab(token);
			current_parser->index = -1;
			current_parser->next = NULL;
		}

		if (!token || token->next == NULL)
            break; // Reached the end of the token linked list
        if (is_type(token, PIPE))
        {
            current_parser->args = NULL;
        }
		if (is_types(token, "TAIH"))
			token = token->next;

        token = token->next;
		while (token && (token->type == ARG || token->type == EMPTY))
			token = token->next;
    }

    // Print the parser linked list for testing
    return (pars);
}
