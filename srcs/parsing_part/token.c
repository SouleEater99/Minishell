/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:03:15 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 11:43:14 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_character(char *line, int *i, t_process_data *data,
		t_token *token)
{
	if (data->c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
	{
		data->c = line[*i];
		token->input[(data->j)++] = line[(*i)++];
	}
	else if (data->c != ' ' && line[*i] == data->c)
	{
		data->c = ' ';
		token->input[(data->j)++] = line[(*i)++];
	}
	else
		token->input[(data->j)++] = line[(*i)++];
}

t_token	*next_token(char *line, int *i)
{
	t_token			*token;
	t_process_data	data;
	size_t			alloc_size;

	data.c = ' ';
	data.j = 0;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	alloc_size = alloc(line, i);
	token->input = malloc(sizeof(char) * alloc_size);
	if (!token->input)
	{
		free(token);
		return (NULL);
	}
	while (line[*i] && (line[*i] != ' ' || data.c != ' '))
		process_character(line, i, &data, token);
	token->input[data.j] = '\0';
	return (token);
}

t_token	*get_tokens(char *line)
{
	t_token	*prev;
	t_token	*next;
	int		i;

	prev = NULL;
	next = NULL;
	i = 0;
	ft_skip_space(line, &i);
	while (line[i])
	{
		next = next_token(line, &i);
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		ft_skip_space(line, &i);
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	ft_memdel(line);
	return (next);
}
