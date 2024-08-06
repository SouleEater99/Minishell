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

size_t ft_strlen_delimiter(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '	')
		i++;
	return (i);
}

char *ft_strjoin_path(char const *s1, char const *s2, char c)
{
	size_t i;
	size_t s1_len;
	char *d;

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

void exit_fork(int status)
{
	int pid;

	pid = fork();
	if (pid == 0)
		exit(status);
	wait(&status);
	printf("+++++++++++++ { before => status : %d } ++++++++++++++++\n", status);
	int status1 = WEXITSTATUS(status);
	printf("+++++++++++++ {  weexitstatus : %d } ++++++++++++++++\n", status1);
	status1 = WIFEXITED(status);
	printf("+++++++++++++ {  wifexed: %d } ++++++++++++++++\n", status1);
	status1 = WIFSIGNALED(status);
	printf("+++++++++++++ {  wifsgnaled : %d } ++++++++++++++++\n", status1);
	printf("+++++++++++ { HEREDOC } +++++++++++++++++++++++\n");
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

int ft_numbers_of_cmd(t_command *cmd)
{
	int count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == PIPE)
			count++;
		cmd = cmd->next;
	}
	return (count);
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
			if (cmd->type == APPEND)
				fd = open(cmd->args[1], O_WRONLY | O_APPEND | O_CREAT, 0666);
			else if (cmd->type == TRUNC)
				fd = open(cmd->args[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
			if (fd < 0)
				ft_free_all("error in open file turnc or append\n", 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				ft_free_all("error in dup2 rederiction append|trunc\n", 1);
			close(fd);
		}
		else if (cmd->type == INF)
		{
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

t_command *ft_return_next_cmd(t_command *cmd)
{
	while (cmd && cmd->type != PIPE)
		cmd = cmd->next;
	if (!cmd)
		return (cmd);
	return (cmd->next);
}

t_command *ft_return_cmd_index(t_command *cmd)
{
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == CMD)
			return (cmd);
		cmd = cmd->next;
	}
	return (NULL);
}

int check_cmd_is_path(char *cmd)
{
	int i;

	i = 0;
	while (cmd[i])
		if (cmd[i++] == '/')
			return (1);
	return (0);
}

int	is_a_directory(char *path)
{
	struct stat path_stat;
	unsigned int flag;

	flag = 0;
	if (stat(path, &path_stat) == -1)
		return (0);
	flag = path_stat.st_mode;
	return (S_ISDIR(flag));
}

void ft_check_path(char *path)
{
	if (!path)
		return;
	if (access(path, F_OK) == 0)
	{
		if (is_a_directory(path) == 1)
		{
			ft_putstr_fd(path, 2);
			ft_free_all(": Is a directory\n", 126);
		}
		else if (access(path, X_OK) != 0)
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
	int i;

	i = 0;
	if (check_cmd_is_path(cmd) == 1)
		return (ft_check_path(data->path), ft_strdup(cmd));
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

int ft_check_is_builtin_child(t_command *cmd)
{
	if (!cmd)
		return (-1);
	if (ft_strcmp(cmd->value, "echo") == 0 || ft_strcmp(cmd->value, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->value, "env") == 0 || ft_strcmp(cmd->value, "export") == 0)
		return (1);
	if (ft_strcmp(cmd->value, "exit") == 0 || ft_strcmp(cmd->value, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd->value, "cd") == 0)
		return (1);
	return (0);
}

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