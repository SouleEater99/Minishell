/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:10:25 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 22:10:06 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

static int	qt(char *line)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i])
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

static int	q_err(t_tools *tools)
{
	t_token	*token;
	int		quote;

	token = tools->cmd;
	quote = 0;
	while (token)
	{
		quote = qt(token->input);
		if (quote != 0)
		{
			ft_putstr_fd("MiniShell: Error Open Quotes !!!", 2);
			return (0);
		}
		token = token->next;
	}
	return (1);
}

int	synatx_err(t_tools *tools)
{
	t_token	*token;
	int		qt_err;

	token = tools->cmd;
	qt_err = q_err(tools);
	if (qt_err == 0)
		return (0);
	while (token)
	{
		if (is_type(token, PIPE) && (!token->prev))
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			token->next ? ft_putstr_fd(token->input, 2) : 0;
			token->next ? 0 : ft_putstr_fd("newline", 2);
			ft_putendl_fd("'", 2);
			return (0);
		}
		if (is_types(token, "TAIPH") && (!token->next || is_types(token->next,
					"P")))
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			token->next ? ft_putstr_fd(token->next->input, 2) : 0;
			token->next ? 0 : ft_putstr_fd("newline", 2);
			ft_putendl_fd("'", 2);
			return (0);
		}
		token = token->next;
	}
	return (1);
}