# include "../libraries/minishell.h"

void    ft_free_tab(char **tab)
{
        int     i;

        i = 0;
        if (tab)
        {
                while (tab[i])
                        free(tab[i++]);
                free(tab);
        }
}

size_t  ft_strlen_delimiter(const char *str)
{
        size_t  i;

        i = 0;
        while (str[i] && str[i] != ' ' && str[i] != '	')
                i++;
        return (i);
}

char    *ft_strjoin_path(char const *s1, char const *s2, char c)
{
        size_t  i;
        size_t  s1_len;
        char    *d;

        if (!s1 || !s2)
                return (NULL);
        s1_len = ft_strlen(s1) + 1;
        d = (char *)malloc(s1_len + ft_strlen_delimiter(s2) + 1);
        if (d == NULL)
                return (NULL);
        i = -1;
        while (s1[++i])
                d[i] = s1[i];
        d[i] = c;
        i = 0;
        while (s2[i] && s2[i] != ' ' && s2[i] != '	')
        {
                d[s1_len + i] = s2[i];
                i++;
        }
        d[s1_len + i] = '\0';
        return (d);
}

int ft_strcmp(char *s1, char *s2)
{
    if (!s1 || !s2)
        return (-1);
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return (*s1 - *s2);
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int		ft_check_heredoc(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == HER_DOC)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

void	ft_close_free_heredoc_pipes()
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

void 	ft_free_all(char *str, int status)
{
	data->i = 0;
	ft_putstr_fd(str, 2);
	if (data->command)
		clear_list(&data->command);
	if (data->pip)
		ft_close_free_heredoc_pipes(data->pip);
	if (data->cwd)
        free(data->cwd);
	if (data->new_env)
        ft_lst_free_env();
	if (data->path)
		free(data->path);
	if (data->tab)
		ft_free_tab(data->tab);
	if (data->command)
		clear_list(&data->command);
	exit(status);
}

int	ft_init_heredoc_pip()
{
	int	i;

	i= 0;
	data->pip = NULL;
	data->n_heredoc = ft_check_heredoc(data->command);
	if (data->n_heredoc <= 0)
		return (data->n_heredoc);
	data->pip = malloc(sizeof(int *) * (data->n_heredoc + 1));
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

void	ft_write_in_pipes(t_command *cmd)
{
	char	*line;

	data->i_pip = 0;
	while (cmd)
	{
		if (cmd->type == HER_DOC)
		{
			line = readline("> ");
			if (!line)
				ft_free_all("error in readline write_to_pip\n", 1);
			while (!line || ft_strcmp(line, cmd->args[0]) != 0)
			{
				if (line)
				{
					write (data->pip[data->i_pip][1], line, ft_strlen(line));
					write (data->pip[data->i_pip][1], "\n", 1);
					free(line);
				}
				line = readline("> ");
			}
			data->i_pip++;
		}
		cmd = cmd->next;
	}
}

void	ft_print_pip_content()
{
	int		i;
	char	*line;

 	i = 0;
	while (i < data->n_heredoc) 
	{
		line = get_next_line(data->pip[i][0]);
		if (!line)
			ft_free_all("error in get_next_line in print pip content\n", 1);
		//ft_memset(buf, 0, sizeof(buf));
		printf("line of pip[%d]: %s \n",i,line);
		close(data->pip[i][0]);
		close(data->pip[i][1]);
		i++;
	}
}

void	ft_heredoc()
{
	if (ft_init_heredoc_pip() == 0)
		return ;
	ft_write_in_pipes(data->command);
	//ft_print_pip_content();
}

int		ft_numbers_of_cmd(t_command *cmd)
{
	int		count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == TOKEN)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

void	ft_setup_dup2(t_command *cmd)
{
	int	fd;

	while (cmd && cmd->type != PIPE)
	{
		printf("cmd.type = %d| cmd.value = %s| cmd.arg[0]= %s\n",cmd->type, cmd->value, cmd->args[0] );
		if (cmd->type == HER_DOC)
		{
			if (dup2(data->pip[data->i_pip++][0], STDIN_FILENO) == -1)
				ft_free_all("error in dup2 heredoc\n", 1);
		}
		else if (cmd->type == APP || cmd->type == RED_OUT)
		{
			if (cmd->type == APP)
				fd = open(cmd->args[0], O_WRONLY| O_APPEND| O_CREAT, 0666);
			else if (cmd->type == RED_OUT)
				fd = open(cmd->args[0], O_WRONLY| O_TRUNC | O_CREAT , 0666);
			if (fd < 0)
				ft_free_all("error in open file turnc or append\n", 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				ft_free_all("error in dup2 rederiction append|trunc\n", 1);
			close(fd);
		}
		else if (cmd->type == RED_IN)
		{
			fd = open(cmd->args[0], O_RDONLY);
			if (fd < 0)
				ft_free_all("error in open file_read_in\n", 1);
			if (dup2(fd, STDIN_FILENO) == -1)
				ft_free_all("error in dup2 red_in\n", 1);
			close(fd);
		}
		cmd = cmd->next;
	}
}

t_command	*ft_return_next_cmd(t_command *cmd)
{
	while (cmd && cmd->type != PIPE)
		cmd = cmd->next;
	if (!cmd)
		return (cmd);
	return (cmd->next);
}

t_command	*ft_return_cmd_index(t_command *cmd)
{
	while (cmd && cmd->type != TOKEN && cmd->type != PIPE)
		cmd = cmd->next;
	return (cmd);
}

int	check_cmd_is_path(char *cmd)
{
	int 	i;

	i = 0;
	while (cmd[i])
		if (cmd[i++] == '/')
			return (1);
	return (0);
}

void	ft_check_path(char *path)
{
	if (!path)
		return ;
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) != 0)
		{
			perror(path);
			ft_free_all(NULL, 126);
		}
	}
	else
	{
		perror(path);
		ft_free_all(NULL, 127);
	}
}

char *fetch_path_of_cmd(t_env *env, char *cmd)
{
	int		i;

	i = 0;
	if (check_cmd_is_path(cmd) == 1)  
		return (ft_check_path(data->path) ,ft_strdup(cmd));
	while (env && ft_strnstr(env->value, "PATH=", 6) == NULL)
		env = env->next;
	if (!env)
		return (ft_strjoin("./", cmd));
	data->tab = ft_split(env->value + 6, ':');
	if (!data->tab)
		ft_free_all("error in split in fetch path\n", 1);
	while (data->tab[i])
	{
		data->path = ft_strjoin_path(data->tab[i++], cmd, '/');
		if (access(data->path, F_OK) == 0)
        	return (data->path);
    	free(data->path);
	}
	return (NULL);
}

int		ft_check_is_builtin_child(t_command *cmd)
{
	if (strcmp(cmd->value, "echo") == 0 || strcmp(cmd->value, "pwd") == 0)
		return (1);
	if (strcmp(cmd->value, "env") == 0 || strcmp(cmd->value, "export") == 0)
		return (1);
	return (0);
}


int		ft_check_is_builtin_parent(t_command *cmd)
{
	int	i;

	i = 0;
	if (strcmp(cmd->value, "exit") == 0 || strcmp(cmd->value, "cd") == 0)
		return (1);
	else if (strcmp(cmd->value, "unset") == 0)
		return (1);
	else if (strcmp(cmd->value, "export") == 0)
	{
		while (cmd->args[i])
			i++;
		if (i > 1)
			return (1);
	}
	return (0);
}

void		ft_execute_builtin_child(t_command *cmd)
{
	int		i;

	i = 0;
	if (ft_strcmp(cmd->value, "env") == 0)
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
			ft_export(cmd->args);
	}
}

void	ft_execute_builtin_parent(t_command *cmd)
{
	int		i;

	i = 0;
	if (ft_strcmp(cmd->value, "cd") == 0)
		ft_cd(cmd->args);
	else if (ft_strcmp(cmd->value, "exit") == 0)
		ft_exit();
	else if (ft_strcmp(cmd->value, "unset") == 0)
		ft_unset(cmd->args[0]);
	else if (ft_strcmp(cmd->value, "export") == 0)
	{
		while (cmd->args[i])
			i++;
		if (i >= 2)
			ft_export(cmd->args);
	}
}

void	ft_execute_cmd(t_command *cmd)
{
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
	data->path = fetch_path_of_cmd(data->new_env, cmd->value);
	if (!data->path)
	{
		perror(cmd->value);
		ft_free_all(NULL, 127);
	}
	ft_check_path(data->path);
	if (cmd && cmd->type == TOKEN )
		execve(data->path, cmd->args, data->old_env);
	ft_free_all("error in execve() in child\n", 1);
}

void	ft_execution()
{
	t_command	*cmd;
	t_command	*cmd_index;
	
	data->save_stdin = dup(STDIN_FILENO);
	data->save_stdout = dup(STDOUT_FILENO);
	ft_heredoc();
	data->i_pip = 0;
	cmd = data->command;
	data->n_cmd = ft_numbers_of_cmd(data->command);
	data->i = 0;
	while (cmd && data->i < data->n_cmd)
	{
		if (pipe(data->pipe_line) == -1)
			ft_free_all("error in open pipe_line \n", 1); // i need to close all pipe of here-doc if they exist.
		cmd_index = ft_return_cmd_index(cmd);
		if (cmd_index && ft_check_is_builtin_parent(cmd_index) == 1)
			ft_execute_builtin_parent(cmd_index);
		else
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
	waitpid(data->pid, &data->exit, 0);
	while (wait(NULL) != -1)
		;
	//sleep(1000);
}



//  what i need to handel : ft_exit with SHELVL minishell inside minishell
//  env if it's NULL
//  export sort with declar-x var=value
//  pwd and oldpwd and shlvl always need to update.
//  cd home and oldpwd
//  exit status
//  expand on heredoc