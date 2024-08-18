/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:10:25 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/15 11:10:49 by samsaafi         ###   ########.fr       */
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


int		synatx_err(t_tools *tools)
{
	t_token	*token;

	token	= tools->cmd;
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
		if (is_types(token, "TAIPH")
		&& (!token->next || is_types(token->next, "TAIPH")))
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
