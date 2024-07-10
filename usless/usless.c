/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usless.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:29:23 by kali              #+#    #+#             */
/*   Updated: 2024/06/12 10:29:26 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void parentheses(char *command)
{
	data->i = -1;
	data->j = -1;
	while(command[++data->i])
	{
		if (command[data->i] == '(')
			++data->j;
		else
		{
			if (data->j == -1)
			{
				printf("minishell: syntax error near unexpected token `)'\n");
				return ;
			}
			if (command[data->i] == ')')
				--data->j;
		}
		if (data->j < -1)
		{
			printf("minishell: syntax error near unexpected token `)'\n");
			return ;
		}
	}
	if (data->j == -1)
		return ;
	char *line = readline("> ");
	data->new_command = ft_strjoin(command, line);
	free(line);
	free(command);
	parentheses(data->new_command);
}

char *skip_command(char *command)
{
	while(*command && ft_isalnum(*command))
		command++;
	command = skip_white_spaces(command);
	if (*command == ';')
		command++;
	return (command);
}

int check_syntax(char *command)
{
	while(*command)
	{
		command = skip_white_spaces(command);
		if (!*command)
			break;
		if (*command == ';')
			printf("minishell: syntax error near unexpected token `%c'\n", *command);
		command = skip_command(command);
		if (!*command)
			break;
		
		printf("%s\n", command);
		
		if (*command == ';')
		{
			command = skip_white_spaces(command);
			if (!*command)
				break;
			else if (*command == ';')
				printf("minishell: syntax error near unexpected token `%c'\n", *command);
		}
		command++;
	}
	return (0);
}