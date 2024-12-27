/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:08:24 by ael-maim          #+#    #+#             */
/*   Updated: 2024/09/08 22:35:50 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_check_heredoc(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == HEREDOC)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

int	ft_init_heredoc_pip(void)
{
	int	i;

	i = -1;
	g_data->pip = NULL;
	g_data->n_heredoc = ft_check_heredoc(g_data->command);
	if (g_data->n_heredoc <= 0)
		return (g_data->n_heredoc);
	g_data->pip = ft_calloc(g_data->n_heredoc + 1, sizeof(int *));
	if (!g_data->pip)
		ft_free_all("error in alloc pip \n", 1);
	g_data->pip[g_data->n_heredoc] = NULL;
	while (++i < g_data->n_heredoc)
	{
		g_data->pip[i] = malloc(sizeof(int) * 2);
		if (!g_data->pip[i])
			ft_free_all("error in alloc pip \n", 1);
	}
	i = -1;
	while (++i < g_data->n_heredoc)
		if (pipe(g_data->pip[i]) == -1)
			ft_free_all("error in open pipe() \n", 1);
	return (g_data->n_heredoc);
}

void	ft_write_and_expand_pipe(t_command *cmd, char *line)
{
	if (cmd->index == -1)
		line = expand_str(line, 0);
	if (line)
	{
		write(g_data->pip[g_data->i_pip][1], line, ft_strlen(line));
		write(g_data->pip[g_data->i_pip][1], "\n", 1);
		free(line);
	}
}

void	ft_write_in_pipes(t_command *cmd)
{
	char	*line;

	signal(SIGINT, ft_sig_handler_child);
	g_data->i_pip = 0;
	while (cmd)
	{
		if (cmd->type == HEREDOC)
		{
			line = readline("> ");
			while (line && ft_strcmp(line, cmd->args[0]) != 0)
			{
				ft_write_and_expand_pipe(cmd, line);
				line = readline("> ");
			}
			if (line)
				free(line);
			g_data->i_pip++;
		}
		cmd = cmd->next;
	}
	ft_free_all(NULL, 0);
}

void	ft_heredoc(void)
{
	int	pid;
	int	status;

	if (ft_init_heredoc_pip() == 0)
		return ;
	pid = fork();
	if (pid < 0)
		ft_free_all("Fork Error in heredoc\n", 2);
	if (pid == 0)
		ft_write_in_pipes(g_data->command);
	wait(&status);
	status = WEXITSTATUS(status);
	if (status == 130)
	{
		ft_free_utils();
		g_data->command = NULL;
	}
}
