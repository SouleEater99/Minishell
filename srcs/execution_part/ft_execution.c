#include "../../include/minishell.h"

int ft_check_is_builtin_parent(t_command *cmd)
{
	int i;

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

void ft_execute_builtin_parent(t_command *cmd)
{
	int i;

	i = 0;
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

void ft_execution()
{
	t_command *cmd;
	t_command *cmd_index;

	data->save_stdin = dup(STDIN_FILENO);
	data->save_stdout = dup(STDOUT_FILENO);
	data->i_pip = 0;
	cmd = data->command;
	data->n_cmd = ft_numbers_of_cmd(data->command);
	data->i = 0;
	while (cmd && data->i <= data->n_cmd)
	{
		if (pipe(data->pipe_line) == -1)
			ft_free_all("error in open pipe_line \n", 1); // i need to close all pipe of here-doc if they exist.
		cmd_index = ft_return_cmd_index(cmd);
		if (cmd_index && data->n_cmd == 0 && ft_check_is_builtin_parent(cmd_index) == 1)
		{
			if (ft_return_next_cmd(cmd) == NULL)
				data->pid = -1;
			ft_execute_builtin_parent(cmd_index);
		}
		else if (cmd)
		{
			data->pid = fork();
			if (data->pid == -1)
				ft_free_all("error in creating child process \n", 1);
			if (data->pid == 0)
				ft_execute_cmd(cmd);
		}
		if (dup2(data->pipe_line[0], STDIN_FILENO) == -1)
			ft_free_all("error in open pipe_line in parent", 1);
		close(data->pipe_line[0]);
		close(data->pipe_line[1]);
		cmd = ft_return_next_cmd(cmd);
		data->i++;
	}
	ft_close_free_heredoc_pipes();
	if (dup2(data->save_stdin, STDIN_FILENO) == -1)
		ft_free_all("error in dup2 of save stdin\n", 1);
	if (data->pid != -1)
	{
		waitpid(data->pid, &data->exit, 0);
		data->exit = WEXITSTATUS(data->exit);
	}
	while (wait(NULL) != -1)
		;
	// sleep(1000);
}

//	-------------------------------------------------------------------
//
//	i need handle new_env to send to execev: ----> DONE
//  what i need to handel : ft_exit with SHELVL minishell inside minishell ---> DONE it's handled by default
//  env if it's NULL ---> DONE
//  pwd and oldpwd and shlvl always need to update. ---> DONE
//  exit status ---> DONE just need test whene it's handle in parsing
//  export sort with declar-x var=value -----> DONE
//	handle export and unset if i delete all env var and i want to create one whene it's NULL ---> DONE
//  cd home and oldpwd -----> DONE
//	still need to handle prompot too dynamic pwd and username and host -----> DONE
//	check SHLVL if DELETE IF can increament again ------> DONE
//	need to free all var i use when i finish every loop ===> DONE (90% maybe i forget something)
//
//	-----------------------------------------------------------------------
//
//	need to check empty command like : ls | "" |pwd
//	need to handel if i use >> a need to create a
//  expand on heredoc