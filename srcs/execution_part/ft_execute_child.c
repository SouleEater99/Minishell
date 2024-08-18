/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:07:36 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:07:37 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_check_is_builtin_child(t_command *cmd)
{
	if (!cmd)
		return (-1);
	else if (ft_strcmp(cmd->value, "echo") == 0 || ft_strcmp(cmd->value,
			"pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "env") == 0 || ft_strcmp(cmd->value,
			"export") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "exit") == 0 || ft_strcmp(cmd->value,
			"unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "cd") == 0)
		return (1);
	return (0);
}

void	ft_execute_builtin_child(t_command *cmd)
{
	int	i;

	i = 0;
	if (ft_strcmp(cmd->value, "unset") == 0)
		ft_unset(cmd->args);
	else if (ft_strcmp(cmd->value, "exit") == 0)
		ft_exit(cmd->args);
	else if (ft_strcmp(cmd->value, "cd") == 0)
		ft_cd(cmd->args);
	else if (ft_strcmp(cmd->value, "env") == 0)
		ft_env();
	else if (ft_strcmp(cmd->value, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->value, "echo") == 0)
		ft_echo(cmd->args);
	else if (ft_strcmp(cmd->value, "export") == 0)
	{
		i = ft_tab_lenght(cmd->args);
		if (i == 1)
		{
			ft_export(cmd->args);
			ft_free_all(NULL, g_data->exit);
		}
	}
	ft_free_all(NULL, g_data->exit);
}

int	ft_execute_cmd(t_command *cmd)
{
	signal(SIGINT, ft_sig_handler_child);
	signal(SIGQUIT, ft_sig_handler_child);
	ft_setup_dup2(cmd);
	cmd = ft_return_cmd_index(cmd);
	close(g_data->pipe_line[0]);
	close(g_data->pipe_line[1]);
	ft_close_free_heredoc_pipes();
	if (ft_check_is_builtin_child(cmd) == 1)
		ft_execute_builtin_child(cmd);
	if (cmd && cmd->type == CMD)
	{
		g_data->path = fetch_path_of_cmd(g_data->new_env, cmd->value);
		if (!g_data->path)
			return (ft_putstr_fd(cmd->value, 2),
				ft_free_all(": command not found\n", 127), 0);
	}
	ft_check_path(g_data->path);
	if (cmd && cmd->type == CMD)
	{
		g_data->exec_env = ft_get_env_in_tab();
		execve(g_data->path, cmd->args, g_data->exec_env);
		ft_free_all("error in execve() in child \n", 1);
	}
	return (ft_free_all(NULL, 0), 0);
}
