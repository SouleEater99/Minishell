/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:30:46 by aelkheta          #+#    #+#             */
/*   Updated: 2024/06/25 20:30:48 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int set_token_type(t_token *token, int type)
{
    token->prev_type = type;
	token->type = type;
	return (type);
}

int syntax_error(char *command)
{
	int i = 0;
	if (ft_strchr("<|>&;", command[i]))
	{
		char special = command[i];
		while(command[++i] && ft_strchr(" \t\v", command[i])); // ++i to skip the char and start from the char after
		if (command[i] == special || (ft_strchr("<|>", special) && !command[i]))
		{
			data->syntax_error = true;
			write(2, "minishell: syntax error near unexpected token ';'\n", 51);
			return (1);
		}
	}
	return (0);
}

char *quoting(char *token_val, t_token *token, char *command)
{
	while(command[token->i] && ft_strchr("'\"", command[token->i]))
	{
		while(command[token->i] && (command[token->i] == '\'' || command[token->i] == '"'))
			token->i++;
		token->index = token->i;
		while(command[token->i] && command[token->i] != '\'' && command[token->i] != '"')
			token->i++;
		char *token_val_ = malloc((token->i - token->index) * sizeof(char) + 1);
		ft_strlcpy(token_val_, &command[token->index], (token->i - token->index + 1));
		token_val = ft_strjoin(token_val, token_val_);
		// token->index = token->i;
	}
	return (token_val);
}

char *get_token_value(t_token *token, char *command)
{
	char *token_val = NULL;
	if (token->index == 0)
		token->prev_type = -1;
	while(command[token->i] && ft_strchr(" \t\v\n", command[token->i]))
		token->i++;
	if (command[token->i] == '\0')
		return (NULL);
	token->index = token->i;
	if (command[token->i] == '\'' || command[token->i] == '"')
	{
		while(command[token->i] && (command[token->i] == '\'' || command[token->i] == '"'))
			token->i++;
		token->index = token->i;
		while(command[token->i] && command[token->i] != '\'' && command[token->i] != '"')
			token->i++;
		token_val = malloc((token->i - token->index) * sizeof(char) + 1);
		ft_strlcpy(token_val, &command[token->index], (token->i - token->index + 1));
		token->index = token->i;
		if (command[token->i] == '\'' || command[token->i] == '"')
			token_val = quoting(token_val, token, command);
		return (token_val);
	}
	// token->index = token->i;
	if (command[token->i] == ';')
	{
		token->i++;
		return (ft_strdup(";"));
	}
	if (command[token->i] && ft_strchr("<|>&;", command[token->i]))
	{
		char special = command[token->i];
		// while(command[token->i] && command[token->i] == command[token->index])
		// 	token->i++;
		while(command[token->i] && command[token->i] == special && token->i - token->index < 2)
			token->i++;
		token_val = malloc((token->i - token->index) * sizeof(char) + 1);
		ft_strlcpy(token_val, &command[token->index], (token->i - token->index + 1));				
		token->index = token->i;
		return (token_val);
	}
	while(command[token->i] && !ft_strchr(" \t\v\n<|>&;", command[token->i])) // ft_isalnum(command[token->i])
		token->i++;
	token_val = malloc((token->i - token->index) * sizeof(char) + 1);
	ft_strlcpy(token_val, &command[token->index], (token->i - token->index + 1));
	token->index = token->i;
	return (token_val);
}

int get_token_type(t_token *token)
{
	if (token->value[0] == '|' && token->value[1] == '\0')
		return (set_token_type(token, PIPE));
	else if (token->value[0] == '>' && token->value[1] == '\0')
		return (set_token_type(token, RED_OUT));
	else if (token->value[0] == '>' && token->value[1] == '>' && token->value[2] == '\0')
		return (set_token_type(token, APP));
	else if (token->value[0] == '<' && token->value[1] == '<' && token->value[2] == '\0')
		return (set_token_type(token, HER_DOC));
	else if (token->value[0] == '<' && token->value[1] == '\0')
		return (set_token_type(token, RED_IN));
	else if (token->value[0] == '&' && token->value[1] == '\0')
		return (set_token_type(token, BACK));
	else if (token->value[0] == '&' && token->value[1] == '&' && token->value[2] == '\0')
		return (set_token_type(token, AND_OP));
	else if (token->value[0] == '|' && token->value[1] == '|' && token->value[2] == '\0')
		return (set_token_type(token, OR_OP));
	else if (token->value[0] == ';' && token->value[1] == '\0')
		return (set_token_type(token, LIST));
	else if (token->prev_type == CMD || token->prev_type == ARG)
		return (set_token_type(token, ARG));
	else if (token->prev_type == RED_IN || token->prev_type == RED_OUT || token->prev_type == APP || token->prev_type == HER_DOC)
		return (set_token_type(token, FLE));
	else if (ft_strisalnum(token->value) && (token->prev_type == -1 || token->prev_type == PIPE || token->prev_type == FLE))
		return (set_token_type(token, CMD));
	// else
	// 	return (-1);
	return (0);
}


void print_type(t_command *tokens)
{
	while (tokens != NULL)
	{
		printf("+---------------------------+\n");
		printf("| value: ----------- %s\n", tokens->value);
		if (tokens->type == CMD)
			printf("| type: ----------- CMD     |\n+---------------------------+\n\n");
		else if (tokens->type == RED_OUT)
			printf("| type: ----------- RED_OUT |\n+---------------------------+\n\n");
		else if (tokens->type == RED_IN)
			printf("| type: ----------- RED_IN  |\n+---------------------------+\n\n");
		else if (tokens->type == APP)
			printf("| type: ----------- APP     |\n+---------------------------+\n\n");
		else if (tokens->type == HER_DOC)
			printf("| type: ----------- HER_DOC |\n+---------------------------+\n\n");
		else if (tokens->type == PIPE)
			printf("| type: ----------- PIPE    |\n+---------------------------+\n\n");
		else if (tokens->type == LIST)
			printf("| type: ----------- LIST    |\n+---------------------------+\n\n");
		else if (tokens->type == BACK)
			printf("| type: ----------- BACK    |\n+---------------------------+\n\n");
		else if (tokens->type == ARG)
			printf("| type: ----------- ARG     |\n+---------------------------+\n\n");
		else if (tokens->type == OR_OP)
			printf("| type: ----------- OR_OP   |\n+---------------------------+\n\n");
		else if (tokens->type == AND_OP)
			printf("| type: ----------- AND_OP  |\n+---------------------------+\n\n");
		else if (tokens->type == FLE)
			printf("| type: ----------- FLE     |\n+---------------------------+\n\n");
		tokens = tokens->next;
		// t_command *ptr = tokens->next;
		// free(tokens->value);
		// free(tokens);
		// tokens = ptr;
	}
}

t_command *tokenize_command(char *commads)
{
	t_token token;
	t_command *head = NULL;
	token.i = 0;
	token.index = 0;
	commads = skip_white_spaces(commads);
	if (commads[token.i] == '\0')
		return NULL;
	while(commads[token.i])
	{
		token.value = get_token_value(&token, commads);
		if (!token.value)
			break;
		token.type = get_token_type(&token);
		add_back_list(&head, new_node(token.type, token.value));
	}
	return (head);
}

int get_args_size(t_command *token)
{
	int i = 0;
	while(token != NULL && token->type == ARG)
	{
		token = token->next;
		i++;
	}
	return (i);
}

void print_args(t_command *token)
{
	int i = 0;
	
	while(token->args != NULL && token->args[i] != NULL)
	{
		printf("	+---------------------------+\n");
		printf("	| arg[%d]: ------------- [%s]\n", i, token->args[i]);
		printf("	+---------------------------+\n");
		free(token->args[i]);
		i++;
	}
	if (token->args != NULL)
		free(token->args);
}

t_command *parser_command(t_command *tokens)
{
	t_command *tmp;
	t_command *ptr = tokens;
	t_command *ptr2 = ptr;
	ptr->args = NULL;
	while(ptr2 != NULL)
	{
		ptr2->args = NULL;
		if (ptr->type == CMD)
		{
			int i = 0;
			ptr2->args = malloc((get_args_size(ptr->next) + 2) * sizeof(char *)); // why + 2 one for the name of the command and ther other for NULL ptr
			ptr2->args[i++] = ft_strdup(ptr->value);
			ptr = ptr->next;
			while(ptr != NULL && ptr->type == ARG)
			{
				ptr2->args[i++] = ptr->value;
				tmp = ptr->next;
				free(ptr);
				ptr = tmp;
			}
			// if (ptr != NULL)
			ptr2->next = ptr;
			ptr2->args[i] = NULL;				
		}
		else if (ptr->type == RED_IN || ptr->type == RED_OUT || ptr->type == APP || ptr->type == HER_DOC)
		{
			if (!ptr->next || ptr->next->type != FLE)
			{
				printf("syntax error\n");
				return NULL;
			}
			ptr2->args = malloc((2) * sizeof(char *)); // why + 2 one for the name of the command and ther other for NULL ptr
			ptr = ptr->next;
			ptr2->next = ptr->next;
			ptr2->args[0] = ptr->value;
			ptr2->args[1] = NULL;
//			free(ptr);
		}
		if (ptr2 != NULL)
			ptr2 = ptr2->next;

		// temp = ptr;
		// ptr = ptr->next;
		// temp->args = NULL;
		// if (!ptr)
		// 	break;
		// if (temp->type == CMD && ptr->type == ARG)
		// {
		// 	int i = 0;
		// 	temp->args = malloc((get_args_size(ptr) + 2) * sizeof(char *)); // why + 2 one for the name of the command and ther other for NULL ptr
		// 	temp->args[i++] = ft_strdup(temp->value);
		// 	while(ptr != NULL && ptr->type == ARG)
		// 	{
		// 		temp->args[i++] = ptr->value;
		// 		t_command *aziz = ptr->next;
		// 		free(ptr);
		// 		ptr = aziz;
		// 	}
		// 	temp->args[i] = NULL;
		// 	temp->next = ptr;
		// }
		// else if (temp->type == RED_OUT || temp->type == RED_IN || temp->type == APP || temp->type == HER_DOC)
		// {
		// 	temp->args = malloc(2 * sizeof(char *));
		// 	temp->args[0] = ptr->value;
		// 	temp->args[1] = NULL;
		// 	t_command *aziz = ptr->next;
		// 	free(ptr);
		// 	ptr = aziz;
		// 	temp->next = ptr;
		// }
	}
	return (tokens);
}

void print_list(t_command *tokens)
{
	int i = 0;

	while(tokens != NULL)
	{
		printf("=> node: %d\n+---------------------------+\n| token: ---------- %s\n", ++i, tokens->value);
		if (tokens->type == CMD)
			printf("| type: ----------- CMD     |\n+---------------------------+\n");
		else if (tokens->type == RED_OUT)
			printf("| type: ----------- RED_OUT |\n+---------------------------+\n");
		else if (tokens->type == RED_IN)
			printf("| type: ----------- RED_IN  |\n+---------------------------+\n");
		else if (tokens->type == APP)
			printf("| type: ----------- APP     |\n+---------------------------+\n");
		else if (tokens->type == HER_DOC)
			printf("| type: ----------- HER_DOC |\n+---------------------------+\n");
		else if (tokens->type == PIPE)
			printf("| type: ----------- PIPE    |\n+---------------------------+\n\n");
		else if (tokens->type == LIST)
			printf("| type: ----------- LIST    |\n+---------------------------+\n\n");
		else if (tokens->type == BACK)
			printf("| type: ----------- BACK    |\n+---------------------------+\n\n");
		else if (tokens->type == ARG)
			printf("| type: ----------- ARG     |\n+---------------------------+\n\n");
		else if (tokens->type == OR_OP)
			printf("| type: ----------- OR_OP   |\n+---------------------------+\n\n");
		else if (tokens->type == AND_OP)
			printf("| type: ----------- AND_OP  |\n+---------------------------+\n\n");
		else if (tokens->type == FLE)
			printf("| type: ----------- FLE     |\n+---------------------------+\n\n");
		print_args(tokens);
		t_command *tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}