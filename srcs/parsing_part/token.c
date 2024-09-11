/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:03:15 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 22:11:43 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_token(t_token **cur, t_token **start, int *flag)
{
	if (*flag == 1)
	{
		while (*start)
		{
			*start = (*start)->next;
			if (*start && (*start)->type == PIPE)
			{
				*cur = *start;
				if ((*start)->next && (*start)->next->type != CMD)
					*flag = 0;
				break ;
			}
		}
	}
	if (*flag == 0)
	{
		if ((*cur)->type == ARG && (*cur)->prev
			&& (*cur)->prev->type == FILENAME)
		{
			(*cur)->type = CMD;
			*flag = 1;
		}
	}
}

void	check_cmd(t_token *token)
{
	t_token	*cur;
	t_token	*start;
	int		flag;

	cur = token;
	start = token;
	if (cur && cur->type == CMD)
		flag = 1;
	else
		flag = 0;
	while (cur)
	{
		process_token(&cur, &start, &flag);
		cur = cur->next;
	}
	cur = token;
}

void	get_tokens_type(t_token *token, int sep)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (ft_strcmp(cur->input, ">") == 0 && sep == 0)
			cur->type = TRUNC;
		else if (ft_strcmp(cur->input, ">>") == 0 && sep == 0)
			cur->type = APPEND;
		else if (ft_strcmp(cur->input, "<") == 0 && sep == 0)
			cur->type = INF;
		else if (ft_strcmp(cur->input, "<<") == 0 && sep == 0)
			cur->type = HEREDOC;
		else if (ft_strcmp(cur->input, "|") == 0 && sep == 0)
			cur->type = PIPE;
		else if (cur->prev == NULL || cur->prev->type >= TRUNC)
			cur->type = CMD;
		else
			cur->type = ARG;
		cur = cur->next;
	}
	get_file_types(token);
	check_cmd(token);
}

void	get_file_types(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->prev && is_types(cur->prev, "TAIH") && !is_types(cur, "TAIHP"))
			cur->type = FILENAME;
		cur = cur->next;
	}
}

static void	process_character(char *line, int *i, char *c, t_token *token,
		int *j)
{
	if (*c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
	{
		*c = line[*i];
		token->input[(*j)++] = line[(*i)++];
	}
	else if (*c != ' ' && line[*i] == *c)
	{
		*c = ' ';
		token->input[(*j)++] = line[(*i)++];
	}
	else
		token->input[(*j)++] = line[(*i)++];
}

t_token	*next_token(char *line, int *i)
{
	t_token	*token;
	int		j;
	char	c;
	size_t	alloc_size;

	c = ' ';
	j = 0;
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
	while (line[*i] && (line[*i] != ' ' || c != ' '))
		process_character(line, i, &c, token, &j);
	token->input[j] = '\0';
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
