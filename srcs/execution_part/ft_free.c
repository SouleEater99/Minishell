#include "../../include/minishell.h"

void ft_free_tab(char **tab)
{
	int i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			free(tab[i++]);
		free(tab);
	}
}

void ft_close_free_heredoc_pipes()
{
	int i;

	i = 0;
	if (data->pip)
	{
		while (data->pip[i])
		{
			close(data->pip[i][0]);
			close(data->pip[i++][1]);
		}
		i = 0;
		while (i < data->n_heredoc)
			free(data->pip[i++]);
		free(data->pip);
		data->pip = NULL;
	}
}

void ft_lst_free_env()
{
	t_env *head;
	t_env *tmp;

	head = data->new_env;
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
	t_command *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free(cmd->value); // i don't need to free it cause it's address come from cmd->args
		ft_free_tab(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}


void	ft_free_utils()
{
	if (data)
	{
		data->i = 0;
		if (data->pip)
			ft_close_free_heredoc_pipes(data->pip);
		if (data->path)
		{
			free(data->path);
			data->path = NULL;
		}
		if (data->tab)
		{
			ft_free_tab(data->tab);
			data->tab = NULL;
		}
		if (data->exec_env)
		{
			ft_free_tab(data->exec_env);
			data->exec_env = NULL;
		}
		if (data->prompt)
		{
			free(data->prompt);
			data->prompt = NULL;
		}
		if (data->command)
		{
		 	ft_free_command(data->command);
			data->command = NULL;
		}
	}
}

void ft_free_all(char *str, int status)
{
	if (data)
	{
		data->i = 0;
		ft_putstr_fd(str, 2);
		if (data->pip)
			ft_close_free_heredoc_pipes(data->pip);
		if (data->new_env)
			ft_lst_free_env();
		if (data->path)
			free(data->path);
		if (data->tab)
			ft_free_tab(data->tab);
		if (data->exec_env)
			ft_free_tab(data->exec_env);
		if (data->prompt)
			free(data->prompt);
		if (data->command)
		 	ft_free_command(data->command);
		free(data);
	}
	exit(status);
}