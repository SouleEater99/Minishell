/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:03:15 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/15 12:28:45 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    get_tokens_type(t_token *token, int sep)
{
	if (ft_strcmp(token->input, ">") == 0 && sep == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->input, ">>") == 0 && sep == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->input, "<") == 0 && sep == 0)
		token->type = INF;
	else if (ft_strcmp(token->input, "<<") == 0 && sep == 0)
		token->type = HEREDOC;
	else if (ft_strcmp(token->input, "|") == 0 && sep == 0)
		token->type = PIPE;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}

t_token *next_token(char *line, int *i)
{
    t_token *token;
    int j = 0;
    char c = ' ';

    if (!(token = malloc(sizeof(t_token))) || !(token->input = malloc(sizeof(char) * alloc(line, i))))
        return (NULL);

    while (line[*i] && (line[*i] != ' ' || c != ' '))
    {
        if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
        {
            c = line[*i];
            token->input[j++] = line[(*i)++];
        }
        else if (c != ' ' && line[*i] == c)
        {
            c = ' ';
            token->input[j++] = line[(*i)++];
        }
        else if (line[*i] == '\\' && (*i)++)
            token->input[j++] = line[(*i)++];
        else
            token->input[j++] = line[(*i)++];
    }

    token->input[j] = '\0';
    return token;
}

t_token	*get_tokens(char *line)
{
	t_token	*prev;
	t_token	*next;
	int		i;
	int		sep;

	prev = NULL;
	next = NULL;
	i = 0;
	ft_skip_space(line, &i);
	while (line[i])
	{
		sep = ignore_sep(line, i);
		next = next_token(line, &i);
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		get_tokens_type(next, sep);
		ft_skip_space(line, &i);
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	ft_memdel(line);
	return (next);
}