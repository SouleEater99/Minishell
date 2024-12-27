/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:09:10 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 11:34:36 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_token(t_parser *node, t_token *token)
{
	if (is_type(token, PIPE))
	{
		node->str = ft_strdup(token->input);
		node->type = token->type;
	}
	else if (is_types(token, "TAIH"))
	{
		node->str = ft_strdup(token->input);
		node->type = token->type;
		if (token->next && token->next->type == FILENAME)
		{
			node->args = malloc(sizeof(char *) * 2);
			if (!node->args)
				return (0);
			node->args[0] = ft_strdup(token->next->input);
			node->args[1] = NULL;
		}
	}
	return (1);
}

static t_parser	*create_parser_node(t_token *token)
{
	t_parser	*node;

	node = malloc(sizeof(t_parser));
	if (!node)
		return (NULL);
	node->str = NULL;
	node->type = -1;
	node->args = NULL;
	node->index = -1;
	node->next = NULL;
	if (!process_token(node, token))
	{
		free(node);
		return (NULL);
	}
	return (node);
}

static void	process_cmd_arg(t_parser *node, t_token *token)
{
	if (is_type(token, CMD))
	{
		node->str = ft_strdup(token->input);
		node->type = token->type;
		node->args = line_tab(token);
	}
}

static t_parser	*add_parser_node(t_parser **pars, t_parser **current_parser,
		t_token *token)
{
	if (!*pars)
	{
		*pars = create_parser_node(token);
		*current_parser = *pars;
	}
	else
	{
		(*current_parser)->next = create_parser_node(token);
		*current_parser = (*current_parser)->next;
	}
	return (*current_parser);
}

t_parser	*fill_parse_struct(t_tools *tools)
{
	t_parser	*pars;
	t_parser	*current_parser;
	t_token		*token;

	pars = NULL;
	current_parser = NULL;
	token = tools->cmd;
	while (token)
	{
		current_parser = add_parser_node(&pars, &current_parser, token);
		if (!current_parser)
			return (NULL);
		process_cmd_arg(current_parser, token);
		while (token->next && (is_type(token->next, ARG)))
			token = token->next;
		token = token->next;
	}
	return (pars);
}
