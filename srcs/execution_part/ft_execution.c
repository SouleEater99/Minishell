/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:07:52 by ael-maim          #+#    #+#             */
/*   Updated: 2024/09/09 22:01:11 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_check_is_builtin_parent(t_command *cmd)
{
	int	i;

	i = 0;
	if (ft_strcmp(cmd->value, "exit") == 0 || ft_strcmp(cmd->value, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "export") == 0)
	{
		while (cmd->args[i])
			i++;
		if (i > 1)
			return (1);
	}
	return (0);
}

void	ft_execute_builtin_parent(t_command *cmd)
{
	int	i;

	i = 0;
	close(g_data->pipe_line[0]);
	ft_setup_dup2(cmd);
	if (ft_strcmp(cmd->value, "cd") == 0)
		ft_cd(cmd->args);
	else if (ft_strcmp(cmd->value, "exit") == 0)
		ft_exit(cmd->args);
	else if (ft_strcmp(cmd->value, "unset") == 0)
		ft_unset(cmd->args);
	else if (ft_strcmp(cmd->value, "export") == 0)
	{
		while (cmd->args[i])
			i++;
		if (i >= 2)
			ft_export(cmd->args);
	}
}

void	ft_setup_execution(t_command *cmd, t_command *cmd_index)
{
	if (cmd_index && g_data->n_cmd == 0
		&& ft_check_is_builtin_parent(cmd_index) == 1)
	{
		if (ft_return_next_cmd(cmd) == NULL)
			g_data->pid = -1;
		ft_execute_builtin_parent(cmd_index);
		close(g_data->pipe_line[1]);
		return ;
	}
	else if (cmd && cmd->value)
	{
		g_data->pid = fork();
		if (g_data->pid == -1)
			ft_free_all("error in creating child process \n", 1);
		if (g_data->pid == 0)
			ft_execute_cmd(cmd);
		ft_get_next_pip(cmd);
	}
	if (dup2(g_data->pipe_line[0], STDIN_FILENO) == -1)
		ft_free_all("error in open pipe_line in parent", 1);
	close(g_data->pipe_line[0]);
	close(g_data->pipe_line[1]);
}

void	ft_wait_child(void)
{
	if (g_data->syn_err == 1 && g_data->pid != -1)
	{
		waitpid(g_data->pid, &g_data->exit, 0);
		if (g_data->exit == 2)
			g_data->exit = 130;
		else if (g_data->exit == 3)
			g_data->exit = 131;
		else
			g_data->exit = WEXITSTATUS(g_data->exit);
	}
	while (wait(NULL) != -1)
		;
}

void	ft_execution(void)
{
	t_command	*cmd;
	t_command	*cmd_index;

	ft_inti_execution();
	cmd = g_data->command;
	while (cmd && g_data->i <= g_data->n_cmd)
	{
		if (pipe(g_data->pipe_line) == -1)
			ft_free_all("error in open pipe_line \n", 1);
		cmd_index = ft_return_cmd_index(cmd);
		ft_setup_execution(cmd, cmd_index);
		cmd = ft_return_next_cmd(cmd);
		g_data->i++;
	}
	ft_close_free_heredoc_pipes();
	if (dup2(g_data->save_stdout, STDOUT_FILENO) == -1)
		ft_free_all("error in dup2 of save stdout\n", 1);
	if (dup2(g_data->save_stdin, STDIN_FILENO) == -1)
		ft_free_all("error in dup2 of save stdin\n", 1);
	ft_wait_child();
}
