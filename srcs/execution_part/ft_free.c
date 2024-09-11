/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:08:15 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:08:16 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			free(tab[i++]);
		free(tab);
	}
}

void	ft_close_free_heredoc_pipes(void)
{
	int	i;

	i = 0;
	if (g_data->pip)
	{
		while (g_data->pip[i])
		{
			close(g_data->pip[i][0]);
			close(g_data->pip[i++][1]);
		}
		i = 0;
		while (i < g_data->n_heredoc)
			free(g_data->pip[i++]);
		free(g_data->pip);
		g_data->pip = NULL;
	}
}

void	ft_lst_free_env(void)
{
	t_env	*head;
	t_env	*tmp;

	head = g_data->new_env;
	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

void	ft_free_command(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->value)
			free(cmd->value);
		ft_free_tab(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}

void	ft_free_all(char *str, int status)
{
	if (g_data)
	{
		g_data->i = 0;
		ft_putstr_fd(str, 2);
		if (g_data->pip)
			ft_close_free_heredoc_pipes();
		if (g_data->new_env)
			ft_lst_free_env();
		if (g_data->path)
			free(g_data->path);
		if (g_data->tab)
			ft_free_tab(g_data->tab);
		if (g_data->exec_env)
			ft_free_tab(g_data->exec_env);
		if (g_data->prompt)
			free(g_data->prompt);
		if (g_data->command)
			ft_free_command(g_data->command);
		free(g_data);
	}
	exit(status);
}
