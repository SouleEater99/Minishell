/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/01 14:45:01 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_data *data = NULL;

void	print_minishell(void)
{
	printf("\n");
	printf("███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n");
	printf("████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n");
	printf("██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n");
	printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n");
	printf("██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███");
	printf("████╗\n");
	printf("╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══");
	printf("════╝\n");
	printf("\n");
}
void	print_prompt(void)
{
	char	*prompt;
	char	*output_color;
	char	*reset_color;

	prompt = "┌──(aziz㉿hostname)-[~/Desktop/minishell]\n└─$ ";
	output_color = GRN;
	reset_color = CRESET;
	write(1, output_color, ft_strlen(output_color));
	write(1, prompt, ft_strlen(prompt));
	write(1, reset_color, ft_strlen(reset_color));
}

char *		get_env_element(char *env_var)
{
	t_env *env = data->env;
	while(env != NULL)
	{
		// printf("%s\n", env_var);
		char *str = ft_strnstr_l(env->value, env_var, ft_strlen(env_var));
		if (str != NULL)
			return (ft_strdup(&str[1]));	
		env = env->next;
	}
	return (ft_strdup(""));
}

char *get_cmd_path(char *cmd_)
{
	int 	i = 0;
	char 	*cmd_path;
	char	*env_ele = get_env_element("PATH");
	char	**path = ft_split(env_ele, ':');
	free(env_ele);
	while(path[i])
	{
		char *cmd = ft_strjoin("/", cmd_);
		cmd_path = ft_strjoin(path[i], cmd);
		free(cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			free_array(path);
			return (cmd_path);
		}
		// printf("%s\n", cmd_path);
		free(cmd_path);
		i++;
	}
	free_array(path);
	return (NULL);
}

int	exec_command(t_command *commands_list)
{
	while (commands_list != NULL)
	{
		pid_t	pid;
		char	*cmd_path;
		if (commands_list->type == TOKEN)
		{
			cmd_path = get_cmd_path(commands_list->value);
			if (cmd_path != NULL)
			{
				pid = fork();
				if (pid == -1)
					return (0);
				else if (pid == 0)
					execve(cmd_path, commands_list->args, data->envirenment);
				else
					wait(NULL);
			}
			else
				printf("%s: command not found\n", commands_list->value);
			free(cmd_path);
		}
		else if (commands_list->type == RED_IN || commands_list->type == RED_OUT)
		{
			open(commands_list->args[0], O_RDWR | O_CREAT | O_TRUNC, 0644);
		}
		commands_list = commands_list->next;
	}
	return (0);
}

void	sig_handler(int signal)
{
	char	*prompt;

	prompt = get_prompt();
	if (signal == SIGQUIT)
		return ;
	if (signal == SIGINT)
		printf("\n%s", prompt);
	free(prompt);
}

void	print_char_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		printf("%s\n", array[i]);
}

char	*get_prompt(void)
{	
	char	*prompt1;
	char	*prompt2;
	char	*prompt3;
	char	*final_prompt;
	prompt1 = BBLU"┌──(aziz㉿aelkheta)-["COLOR_RESET;
	prompt2 = getcwd(NULL, 0);
	prompt3 = ft_strjoin(BWHT, prompt2);
	free(prompt2);
	prompt1 = ft_strjoin(prompt1, prompt3);
	free(prompt3);
	final_prompt = ft_strjoin(prompt1, BBLU"]\n└─$ "COLOR_RESET);
	free(prompt1);
	return (final_prompt);
}

void	add_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

t_env	*lstnew(char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = content;
	new_node->next = NULL;
	return (new_node);
}


t_env *creat_env(char **env)
{
	t_env *head = NULL;
	int i = -1;
	while(env[++i] != NULL)
		add_back(&head, lstnew(env[i]));
	return (head);
}

void	init_minishell(int ac, char **av, char **env)
{
	//"┌──(aziz㉿aelkheta)-[/nfs/homes/aelkheta/Desktop/minishell]\n└─$ ";
	data->ac = ac;
	// data->env = creat_env(env);
	ft_create_new_env();
	data->envirenment = env;
	data->av = av;
	data->syntax_error = false;
	data->prompt = get_prompt();
	data->new_command = NULL;
}

int	main(int ac, char **av, char **env)
{
	char	*command;
	
	data = (t_data *)calloc(1, sizeof(t_data));
	data->old_env = env;
	init_minishell(ac, av, env);	
	print_minishell();
	
	signal(SIGQUIT, sig_handler);
	// signal(SIGINT, sig_handler);
	
	command = readline(data->prompt);
	while (command != NULL)
	{
		add_history(command);
		parse_command(command);
		// free(command);
		command = readline(data->prompt);
	}
	ft_env();
	clear_history();
	free(data->new_command);
	free(data->prompt);
	return (0);
}
