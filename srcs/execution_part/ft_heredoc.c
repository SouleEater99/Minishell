#include "../../include/minishell.h"


int ft_check_heredoc(t_command *cmd)
{
	int count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == HEREDOC)
			count++;
		cmd = cmd->next;
	}
	return (count);
}



int ft_init_heredoc_pip()
{
	int i;

	i = 0;
	data->pip = NULL;
	data->n_heredoc = ft_check_heredoc(data->command);
	if (data->n_heredoc <= 0)
		return (data->n_heredoc);
	if (data->n_heredoc > 16)
		ft_free_all("bash: maximum here-document count exceeded", 2);
	data->pip = ft_calloc(data->n_heredoc + 1, sizeof(int *));
	if (!data->pip)
		ft_free_all("error in alloc pip \n", 1);
	data->pip[data->n_heredoc] = NULL;
	while (i < data->n_heredoc)
	{
		data->pip[i] = malloc(sizeof(int) * 2);
		if (!data->pip[i])
			ft_free_all("error in alloc pip \n", 1);
		i++;
	}
	i = -1;
	while (++i < data->n_heredoc)
		if (pipe(data->pip[i]) == -1)
			ft_free_all("error in open pipe() \n", 1); // still i need to close the pipesa
	return (data->n_heredoc);
}

void ft_write_in_pipes(t_command *cmd)
{
	char *line;

	signal(SIGINT, ft_sig_handler_child);
	data->i_pip = 0;
	while (cmd)
	{
		if (cmd->type == HEREDOC)
		{
			line = expand_str(readline("> "), data->new_env);
			while (line && ft_strcmp(line, cmd->args[1]) != 0)
			{
				if (line)
				{
					write(data->pip[data->i_pip][1], line, ft_strlen(line));
					write(data->pip[data->i_pip][1], "\n", 1);
					free(line);
				}
				line = expand_str(readline("> "), data->new_env);
			}
			data->i_pip++;
		}
		cmd = cmd->next;
	}
    ft_putstr_fd("++++++++++++++++++++++++++\n",2);
	ft_free_all(NULL, 0);
}

void ft_print_pip_content()
{
	int i;
	char *line;

	i = 0;
	while (i < data->n_heredoc)
	{
		line = get_next_line(data->pip[i][0]);
		if (!line)
			ft_free_all("error in get_next_line in print pip content\n", 1);
		// ft_memset(buf, 0, sizeof(buf));
		printf("line of pip[%d]: %s \n", i, line);
		close(data->pip[i][0]);
		close(data->pip[i][1]);
		i++;
	}
}



void ft_heredoc()
{
	int	pid;
	int status;

	if (ft_init_heredoc_pip() == 0)
		return;
	pid = fork();
	if (pid < 0)
		ft_free_all("Fork Error in heredoc\n", 2);
	if (pid == 0)
		ft_write_in_pipes(data->command);
	wait(&status);
	status = WEXITSTATUS(status);
	if (status == 130)
	{
		ft_free_utils();
		data->command = NULL;
	}
	// ft_print_pip_content();
}
