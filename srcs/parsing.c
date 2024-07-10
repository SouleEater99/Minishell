/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/02 11:49:57 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char *skip_white_spaces(char *command)
{
	while(*command && (*command == ' ' || *command == '\t' || *command == '\r' || *command == '\v'))
		command++;
	return (command);
}
int check_unqoted(char *line)
{
	int i = 0;
	// printf("line: %s\n", line);
	while(line[i])
	{
		if (line[i] == '\'')
		{
			while(line[++i] && line[i] != '\'');
			if (!line[i])
				return (1);
			i++;
		}
		else if (line[i] == '"')
		{
			while(line[++i] && line[i] != '"');
			if (!line[i])
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}

char *lexer_command(char *line)
{
	int i = 0;
	int j = 0;
	char *trimed_line = ft_strtrim(line, " \t\n\v");
	free(line);
	char *unquoted_line = ft_calloc(ft_strlen(trimed_line) + 1, sizeof(char));
	while(trimed_line[i])
	{
		if (trimed_line[i] && trimed_line[i] == '\'')
		{
			if (trimed_line[i + 1] == '\'')
				i += 2;
			else //if (trimed_line[i + 1] == '"')
			{
				unquoted_line[j++] = trimed_line[i++];
				while(trimed_line[i] && trimed_line[i] != '\'')
					unquoted_line[j++] = trimed_line[i++];
				if (!trimed_line[i])
					break;
				unquoted_line[j++] = trimed_line[i++];
			}
			// else
			// 	unquoted_line[j++] = trimed_line[i++];
		}
		else if (trimed_line[i] && trimed_line[i] == '"')
		{
			if (trimed_line[i + 1] == '"')
				i += 2;
			else //if (trimed_line[i + 1] == '\'')
			{
				unquoted_line[j++] = trimed_line[i++];
				while(trimed_line[i] && trimed_line[i] != '"')
					unquoted_line[j++] = trimed_line[i++];
				if (!trimed_line[i])
					break;
				unquoted_line[j++] = trimed_line[i++];
			}
			// else
			// 	unquoted_line[j++] = trimed_line[i++];
		}
		else
			unquoted_line[j++] = trimed_line[i++];
	}
	free(trimed_line);
    unquoted_line[j] = '\0';
	if (check_unqoted(unquoted_line))
	{
		printf("syntax error\n");
		free(unquoted_line);
		unquoted_line = NULL;
	}
	return (unquoted_line);
}
char *get_token(char *command_line, int *i)
{
	int j = 0;
	char *token_val = NULL;
	j = *i;
	while(command_line[*i])
	{
		while(command_line[j] && ft_strchr(" \t\v", command_line[j]))
			j++;
		*i = j;
		if (command_line[j] == '\'' || command_line[j] == '"')
		{
			char quote = command_line[j++];
			while(command_line[j] && command_line[j] != quote)
				j++;
			if (command_line[j] == quote)
				j++;
		}
		else if (command_line[j] == '<' || command_line[j] == '>' || command_line[j] == '|')
		{
			char special = command_line[j];
			while (command_line[j] && command_line[j] == special)
				j++;
			token_val = malloc((j - *i) * sizeof(char) + 1);
			ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
			*i = j;
			return (token_val);
		}
		while(command_line[j] && !ft_strchr(" \t\v<|>", command_line[j]))
		{
			if (command_line[j] == '\'' || command_line[j] == '"')
			{
				char quote = command_line[j];
				while(command_line[++j] && command_line[j] != quote);
				if (command_line[j] == quote)
					j++;
			}	
			else if (command_line[j])
				j++;
		}
		token_val = malloc((j - *i) * sizeof(char) + 1);
		ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
		*i = j;
		return (token_val);
	}
	return (NULL);
}

void print_type(int type)
{
	if (type == -1)
		printf("| type: ----------- ERROR   |\n+---------------------------+\n");
	else if (type == TOKEN)
		printf("| type: ----------- TOKEN   |\n+---------------------------+\n");
	else if (type == RED_OUT)
		printf("| type: ----------- RED_OUT |\n+---------------------------+\n");
	else if (type == RED_IN)
		printf("| type: ----------- RED_IN  |\n+---------------------------+\n");
	else if (type == APP)
		printf("| type: ----------- APP     |\n+---------------------------+\n");
	else if (type == HER_DOC)
		printf("| type: ----------- HER_DOC |\n+---------------------------+\n");
	else if (type == PIPE)
		printf("| type: ----------- PIPE    |\n+---------------------------+\n\n");
	else if (type == LIST)
		printf("| type: ----------- LIST    |\n+---------------------------+\n\n");
	else if (type == BACK)
		printf("| type: ----------- BACK    |\n+---------------------------+\n\n");
	else if (type == ARG)
		printf("| type: ----------- ARG     |\n+---------------------------+\n\n");
	else if (type == OR_OP)
		printf("| type: ----------- OR_OP   |\n+---------------------------+\n\n");
	else if (type == AND_OP)
		printf("| type: ----------- AND_OP  |\n+---------------------------+\n\n");
	else if (type == FLE)
		printf("| type: ----------- FLE     |\n+---------------------------+\n\n");
}

void print_args(char **args)
{
	int i = 0;
	while(args != NULL && *args)
	{
		printf("	+---------------------------+\n");
		printf("	| arg[%d]: --------- [%s]\n", i++, *args++);
		printf("	+---------------------------+\n");
	}
}

void print_list(t_command *table)
{
	while (table != NULL)
	{
		printf("+---------------------------+\n");
		printf("| token: ---------- [%s]\n", table->value);
		print_type(table->type);
		print_args(table->args);
		table = table->next;
	}
}

int get_token_type(char *token)
{
	if (token[0] == '|' && !token[1])
		return (PIPE);
	if (token[0] == '|' && token[1] == '|' && !token[2])
		return (OR_OP);
	else if (token[0] == '>' && !token[1])
		return (RED_OUT);
	else if (token[0] == '<' && !token[1])
		return (RED_IN);
	else if (token[0] == '>' && token[1] == '>' && !token[2])
		return (APP);
	else if (token[0] == '<' && token[1] == '<' && !token[2])
		return (HER_DOC);
	else if (token[0] == '&' && !token[1])
		return (BACK);	
	else if (token[0] == '&' && token[1] == '&' && !token[2])
		return (AND_OP);
	else if (ft_strchr("<|&>", token[0]) && ft_strlen(token) > 2)
		return (-1);
	else
		return (TOKEN);
}

t_command *tokenzer_command(char *command_line)
{
	int i = 0;
	t_command *table = NULL;
	if (!command_line)
		return NULL;
	while(command_line[i])
	{
		char *token = get_token(command_line, &i);
		int type = get_token_type(token);
		t_command *node = new_node(type, token);
		node->args = NULL;
		add_back_list(&table, node);
	}
	free(command_line);
	return table;
}

t_command *free_node(t_command *_tokens_list)
{
	if (!_tokens_list)
		return (NULL);
	t_command *ptr = _tokens_list->next;
	free(_tokens_list->value);
	free(_tokens_list);
	_tokens_list = NULL;
	return (ptr);
}
int get_args_size(t_command *list)
{
	int i = 0;
	while(list != NULL)
	{
		if (list->type == PIPE)
			return (i);
		if (list->type == TOKEN)
			i++;
		if (list->type == RED_IN || list->type == RED_OUT || list->type == HER_DOC || list->type == APP)
			i--;
		list = list->next;
	}	
	return (i);
}

t_command *redirect_list(t_command **head ,t_command *list_command, t_command *_tokens_list, t_command **redirect_head)
{
	t_command *redirection_node = new_node(_tokens_list->type, ft_strdup(_tokens_list->value));
	_tokens_list = free_node(_tokens_list);
	if (!_tokens_list || _tokens_list->type == PIPE)
	{
		printf("syntax error parser\n");
		free_node(redirection_node);
		free_array(list_command->args);
		free_node(list_command);
		free_node(_tokens_list);
		clear_list(head);
		return (NULL);
	}
	redirection_node->args = malloc(2 * sizeof(char *));
	redirection_node->args[0] = ft_strdup(_tokens_list->value);
	redirection_node->args[1] = NULL;
	_tokens_list = free_node(_tokens_list);
	add_back_list(redirect_head, redirection_node);
	return (_tokens_list);
}

t_command *parser_command(t_command *_tokens_list)
{
	int i;
	// int red_flag = 0;
	t_command *head = NULL;
	t_command *list_command;
	
	while(_tokens_list != NULL)
	{
		list_command = new_node(_tokens_list->type, ft_strdup(_tokens_list->value));
		list_command->args = NULL;
		t_command *rdrct_head = NULL;
		if (_tokens_list->type == TOKEN)
		{
			i = 0;
			list_command->args = malloc((get_args_size(_tokens_list) + 1) * sizeof(char *));
			while(_tokens_list != NULL && _tokens_list->type != PIPE)
			{
				// red_flag = 0;
				list_command->args[i++] = ft_strdup(_tokens_list->value);
				list_command->args[i] = NULL;
				_tokens_list = free_node(_tokens_list);
				while (_tokens_list != NULL && (_tokens_list->type == RED_IN || _tokens_list->type == RED_OUT || _tokens_list->type == APP || _tokens_list->type == HER_DOC))
				{
					// red_flag = 1;
					_tokens_list = redirect_list(&head, list_command, _tokens_list, &rdrct_head);
					// if (!_tokens_list)
					// 	return (NULL);
				}
			}
		}
		else
		{
			_tokens_list = free_node(_tokens_list);
			if (!_tokens_list)
			{
				printf("syntax error\n");
				free_node(list_command);
				clear_list(&head);
				return (NULL);
			}
		}
		add_back_list(&head, list_command);
		add_back_list(&head, rdrct_head);
	}
	return (head);
}

char *get_var(char *env_var, int *i)
{
	int k = 0;

	if (!env_var || !env_var[0])
		return (NULL);
	while(env_var[k] && env_var[k] != ' ' && env_var[k] != '$' && env_var[k] != ')')
		k++;
	*i += k;
	char *env_val = malloc((k + 1) * sizeof(char));
	ft_strlcpy(env_val, env_var, k + 1);
	return(env_val);
}

char *unquote_arg(t_command *list ,char *arg)
{
	int j = 0;
	int k = 0;
	char quote;
	char *argument = ft_calloc(ft_strlen(arg) + 1, sizeof(char));
	while(arg[j])
	{
		if (arg[j] == '\'' || arg[j] == '"')
		{
			quote = arg[j++];
			while(arg[j] && arg[j] != quote)
				argument[k++] = arg[j++];
			j++;
		}
		else
			argument[k++] = arg[j++];
		if (arg[0] == quote && quote == '\'')
			list->quoted = 1;
		else if (arg[0] == quote && quote == '"')
			list->quoted = 2;
	}
	free(arg);
	return (argument);
}

char	*ft_strnstr_l(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && little[j] && (i + j) < len)
			j++;
		if (little[j] == '\0')
			return ((char *)&big[i + j]);
		i++;
	}
	return (NULL);
}


char *get_word(char *argument, int *i)
{
	char *str = NULL;
	int j = *i;
	while(argument[*i] && argument[*i] != '$')
		(*i)++;
	str = malloc((*i - j + 1) *sizeof(char));
	ft_strlcpy(str, &argument[j], (*i - j + 1));
	// printf("str: %s\n", str);
	return (str);
}

char *expand_vars(char *argument)
{
	int i = 0;
	char *expanded = "";
	while(argument[i])
	{
		if (argument[i] == '$')
		{
			char *str = get_var(&argument[++i], &i);
			// printf("exp: %s\n", str);
			str = get_env_element(str);
			expanded = ft_strjoin(expanded, str);
			// printf("exp after: %s\n", expanded);
		}
		else
		{
			char *str = get_word(argument, &i);
			expanded = ft_strjoin(expanded, str);
			free(str);
		}
	}
	free(argument);
	return (expanded);
}


t_command *expander_command(t_command *list)
{
	int i;
	t_command *head = list;
	if (list != NULL && (list->type == PIPE || list->type == OR_OP))
	{
		printf("syntax error\n");
		clear_list(&head);
		return (NULL);
	}
	while(list != NULL)
	{
		i = 0;
		list->quoted = 0;
		if (list->type == -1)
		{
			printf("syntax error\n");
			clear_list(&head);
			return (NULL);
		}	
		while(list->args != NULL && list->args[i] != NULL)
		{
			list->value = unquote_arg(list ,list->value);
			list->args[i] = unquote_arg(list ,list->args[i]);
			if (list->quoted != 1)
				list->args[i] = expand_vars(list->args[i]);
			i++;
		}
		list = list->next;
	}
	return (head);
}

int	parse_command(char *line)
{
	// printf("line befor lexer: %s\n", line);
	line = lexer_command(line);
	// if (line != NULL && line[0])
		// printf("line after lexer: %s\n", line);
	t_command *tokens_list = tokenzer_command(line);
	// print_list(tokens_list);
	t_command *list = parser_command(tokens_list);
	// print_list(list);
	list = expander_command(list);
	print_list(list);
	printf("\n\n");
	

	// for execute commands
	// exec_command(list);
	data->command = list;
	ft_execution();

	clear_list(&list);
	return (0);
}	