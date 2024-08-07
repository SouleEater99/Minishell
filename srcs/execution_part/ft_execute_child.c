#include "../../include/minishell.h"


int ft_check_is_builtin_child(t_command *cmd)
{
	if (!cmd)
		return (-1);
	else if (ft_strcmp(cmd->value, "echo") == 0 || ft_strcmp(cmd->value, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "env") == 0 || ft_strcmp(cmd->value, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "exit") == 0 || ft_strcmp(cmd->value, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->value, "cd") == 0)
		return (1);
	return (0);
}

void ft_execute_builtin_child(t_command *cmd)
{
	int i;

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
		while (cmd->args[i])
			i++;
		if (i == 1)
		{
			ft_export(cmd->args);
			ft_free_all(NULL, data->exit);
		}
	}
	ft_free_all(NULL, data->exit);
}

void ft_setup_dup2(t_command *cmd)
{
	int fd;

	while (cmd && cmd->type != PIPE)
	{
		printf("cmd.type = %d| cmd.value = %s| cmd.arg[0]= %s\n", cmd->type, cmd->value, cmd->args[0]);
		if (cmd->type == HEREDOC)
		{
			if (dup2(data->pip[data->i_pip++][0], STDIN_FILENO) == -1)
				ft_free_all("error in dup2 heredoc\n", 1);
		}
		else if (cmd->type == APPEND || cmd->type == TRUNC)
		{
			if (!cmd->args[1])
				ft_free_all(": ambiguous redirect\n", 1);
			if (cmd->type == APPEND)
				fd = open(cmd->args[1], O_WRONLY | O_APPEND | O_CREAT, 0666);
			else if (cmd->type == TRUNC)
				fd = open(cmd->args[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
			if (fd < 0)
				ft_free_all("\nerror in open file turnc or append\n", 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				ft_free_all("error in dup2 rederiction append|trunc\n", 1);
			close(fd);
		}
		else if (cmd->type == INF)
		{
			if (!cmd->args[1])
				ft_free_all(": ambiguous redirect\n", 1);
			fd = open(cmd->args[1], O_RDONLY);
			if (fd < 0)
				ft_free_all("error in open file_read_in\n", 1);
			if (dup2(fd, STDIN_FILENO) == -1)
				ft_free_all("error in dup2 red_in\n", 1);
			close(fd);
		}
		cmd = cmd->next;
	}
}


void ft_execute_cmd(t_command *cmd)
{
	signal(SIGINT, ft_sig_handler_child);
	signal(SIGQUIT, ft_sig_handler_child);
	if (ft_return_next_cmd(cmd) != NULL)
		if (dup2(data->pipe_line[1], STDOUT_FILENO) == -1)
			ft_free_all("error in dup2 red_out in child\n", 1);
	ft_setup_dup2(cmd);
	cmd = ft_return_cmd_index(cmd);
	close(data->pipe_line[0]);
	close(data->pipe_line[1]);
	ft_close_free_heredoc_pipes();
	if (ft_check_is_builtin_child(cmd) == 1)
		ft_execute_builtin_child(cmd);
	if (cmd && cmd->type == CMD)
	{
		data->path = fetch_path_of_cmd(data->new_env, cmd->value);
		if (!data->path)
		{
			ft_putstr_fd(cmd->value, 2);
			ft_free_all(": command not found\n", 127);
		}
	}
	ft_check_path(data->path);
	if (cmd && cmd->type == CMD)
	{
		data->exec_env = ft_get_env_in_tab();
		execve(data->path, cmd->args, data->exec_env);
		ft_free_all("error in execve() in child \n", 1);
	}
	ft_free_all(NULL, 0);
}