/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:56 by samsaafi          #+#    #+#             */
/*   Updated: 2024/06/01 09:29:30 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_data *data = NULL;

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != c)
	{
		if (s[i] == '\0')
			return (NULL);
		i++;
	}
	return ((char *)s + i);
}

void    get_tokens_type(t_token *token, int sep)
{
    if((ft_strcmp(token->input, "")) == 0)
        token->type = EMPTY;
	else if (ft_strcmp(token->input, ">") == 0 && sep == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->input, ">>") == 0 && sep == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->input, "<") == 0 && sep == 0)
		token->type = INF;
	else if (ft_strcmp(token->input, "<<") == 0 && sep == 0)
		token->type = HEREDOC;
	else if (ft_strcmp(token->input, "|") == 0 && sep == 0)
		token->type = PIPE;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}


int alloc(char *line, int *start)
{
    int     count;
    int     i;
    char    c;
    
    count = 0;
    i = 0;
    c = ' ';
    while (line[*start + i] && (line[*start + i] != ' ' || c != ' '))
    {
        if (c == ' ' && (line[*start + i] == '\'' || line[*start + i] == '\"'))
            c = line[*start + i++];
        else if (c != ' ' && line[*start + i] == c)
        {
            count += 2;
            c = ' ';
            i++;
        }
        else
            i++;
        if (line[*start + i - 1] == '\\')
            count--;
    }
    return (i - count + 1);
}

t_token	*next_token(char *line, int *i)
{
	t_token	*token;
	int		j;
	char	c;

	j = 0;
	c = ' ';
	if (!(token = malloc(sizeof(t_token)))
	|| !(token->input = malloc(sizeof(char) * alloc(line, i)))) //memory allocated here!!!!!!!!!!!!!!!!!!!!!!!!
		return (NULL);
	while (line[*i] && (line[*i] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
			c = line[(*i)++];
		else if (c != ' ' && line[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (line[*i] == '\\' && (*i)++)
			token->input[j++] = line[(*i)++];
		else
			token->input[j++] = line[(*i)++];
	}
	token->input[j] = '\0';
	return (token);
}

void	ft_skip_space(const char *str, int *i)
{
	while ((str[*i] == ' ' || str[*i] == '\t')
	|| (str[*i] == '\r' || str[*i] == '\v' || str[*i] == '\f'))
		(*i)++;
}

int		ignore_sep(char *line, int i)
{
	if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == ';')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '|')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>'
				&& line[i + 2] && line[i + 2] == '>')
		return (1);
	else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '<'
				&& line[i + 2] && line[i + 2] == '<')
		return (1);
	return (0);
}

int		quotes(char *line, int index)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i] && i != index)
	{
		if (i > 0 && line[i - 1] == '\\')
			;
		else if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 0;
		else if (open == 2 && line[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}

// char *ft_strnstr(const char *haystack, const char *needle, size_t len)
// {
//     size_t needle_len = strlen(needle);
//     if (needle_len == 0)
//         return (char *)haystack;

//     while (*haystack && len >= needle_len) {
//         size_t i = 0;
//         do {
//             if (needle[i] == '\0')
//                 return (char *)haystack;
//         } while (haystack[i] == needle[i++] && i < len);

//         haystack++;
//         len--;
//     }

//     return NULL;
// }

int is_sep(char *line, int i)
{
    if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
        return 0;
    else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
        return 1;
    else
        return 0;
}

char	*allocate_line(char *line)
{
	char	*new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		i++;
	}
	if (!(new = malloc(sizeof(char) * (i + 2 * count + 1))))//memory allocated here !!!!!!!!!!!!!!!!!!!!!!1111
		return (NULL);
	return (new);
}

char	*formate_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = allocate_line(line); //memory allocated here!!!!!!!!!!!!
	while (new && line[i])
	{
		if (quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
			new[j++] = line[i++];
		if (quotes(line, i) == 0 && line[i] == '|')
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			new[j++] = ' ';
		}
		else if (quotes(line, i) == 0 && is_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '<')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
	printf("%s\n", new);
	//ft_memdel(line);
	return (new);
}

t_token	*get_tokens(char *line)
{
	t_token	*prev;
	t_token	*next;
	int		i;
	int		sep;

	prev = NULL;
	next = NULL;
	i = 0;
	ft_skip_space(line, &i);
	while (line[i])
	{
		sep = ignore_sep(line, i);
		next = next_token(line, &i);
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		get_tokens_type(next, sep);
		ft_skip_space(line, &i);
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	return (next);
}

t_token	*prev_sep(t_token *token, int skip)
{
	if (token && skip)
		token = token->prev;
	while (token && token->type < TRUNC)
		token = token->prev;
	return (token);
}
int		is_type(t_token *token, int type)
{
	if (token && token->type == type)
		return (1);
	else
		return (0);
}

int		is_last_valid_arg(t_token *token)
{
	t_token	*prev;

	if (!token || is_type(token, CMD) || is_type(token, ARG))
	{
		prev = prev_sep(token, 0);
		if (!prev || is_type(prev, PIPE))
			return (1);
		return (0);
	}
	else
		return (0);
}

int		is_types(t_token *token, char *types)
{
	if (ft_strchr(types, ' ') && is_type(token, EMPTY))
		return (1);
	else if (ft_strchr(types, 'X') && is_type(token, CMD))
		return (1);
	else if (ft_strchr(types, 'x') && is_type(token, ARG))
		return (1);
	else if (ft_strchr(types, 'T') && is_type(token, TRUNC))
		return (1);
	else if (ft_strchr(types, 'A') && is_type(token, APPEND))
		return (1);
	else if (ft_strchr(types, 'I') && is_type(token, INF))
		return (1);
	else if (ft_strchr(types, 'P') && is_type(token, PIPE))
		return (1);
	else if (ft_strchr(types, 'H') && is_type(token, HEREDOC))
		return (1);
	return (0);
}

char	**line_tab(t_token *start)
{
	t_token	*token;
	char	**tab;
	int		i;

	if (!start)
		return (NULL);
	token = start->next;
	i = 2;
	while (token && token->type < TRUNC)
	{
		token = token->next;
		i++;
	}
	if (!(tab = malloc(sizeof(char *) * i)))
		return (NULL);
	token = start->next;
	tab[0] = start->input;
	i = 1;
	while (token && token->type < TRUNC)
	{
		tab[i++] = token->input;
		token = token->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	line_args(t_tools *mini)
{
	t_token	*token;
	t_token	*prev;

	token = mini->cmd;
	while (token)
	{
		prev = prev_sep(token, 0);
		if (is_type(token, ARG) && is_types(prev, "TAIH"))
		{
			while (is_last_valid_arg(prev) == 0)
				prev = prev->prev;
			token->prev->next = token->next;
			if (token->next)
				token->next->prev = token->prev;
			token->prev = prev;
			token->next = (prev) ? prev->next : mini->cmd;
			prev = (prev) ? prev : token;
			prev->next->prev = token;
			prev->next = (mini->cmd->prev) ? prev->next : token;
			mini->cmd = (mini->cmd->prev) ? mini->cmd->prev : mini->cmd;
		}
		token = token->next;
	}
}


/***********************************************************************************************************/

t_parser *fill_parse_struct(t_tools *tools)
{
    t_parser *pars = NULL;
    t_parser *current_parser = NULL;
    t_token *token = tools->cmd;

	token->type = CMD;
    while (token)
    {
		if (!pars)
        {
            pars = malloc(sizeof(t_parser));
            current_parser = pars;
        }
        else
        {
            current_parser->next = malloc(sizeof(t_parser));
            current_parser = current_parser->next;
        }
        // Set the values for the current parser node
		if (token && token->type != ARG)
		{
			current_parser->str = token->input;
			current_parser->type = token->type;
			current_parser->args = line_tab(token);
			current_parser->index = -1;
			current_parser->next = NULL;
		}

		if (!token || token->next == NULL)
            break; // Reached the end of the token linked list
        if (is_type(token, PIPE))
        {
            current_parser->args = NULL;
        }
		if (is_types(token, "TAIH"))
			token = token->next;

        token = token->next;
		while (token && token->type == ARG)
			token = token->next;
    }

    // Print the parser linked list for testing
    current_parser = pars;
    while (current_parser)
    {
        printf("str: %s type: %d args: ", current_parser->str, current_parser->type);
        if (current_parser->args)
        {
            char **arg = current_parser->args;
            while (*arg)
            {
                printf("\"%s\" ", *arg);
                arg++;
            }
        }
        else
        {
            printf("empty ");
        }
        printf(" --> ");
        
        current_parser = current_parser->next;
    }
    printf("NULL\n");
    return (pars);
}


/***********************************************************************************************************/


//syntax error

// void	ft_putendl_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (s == NULL)
// 		return ;
// 	while (s[i] != '\0')
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// 	write(fd, "\n", 1);
// }

// void	ft_putstr_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (s == NULL)
// 		return ;
// 	while (s[i] != '\0')
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// }

// int		synatx_err(t_tools *mini, t_token *token)
// {
// 	while (token)
// 	{
// 		if (is_types(token, "TAI")
// 		&& (!token->next || is_types(token->next, "TAIPE")))
// 		{
// 			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
// 			token->next ? ft_putstr_fd(token->next->input, 2) : 0;
// 			token->next ? 0 : ft_putstr_fd("newline", 2);
// 			ft_putendl_fd("'", 2);
// 			return (0);
// 		}
// 		token = token->next;
// 	}
// 	return (1);
// 

// +++++++++++++++++++++++++++++ { from here i started to merge} ++++++++++++++++++++++++++++++++++++

void	ft_add_back_cmd(t_command *command)
{
	t_command *tmp;

	if (!data->command || !command)
		return ;
	tmp = data->command;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = command;
}

t_command *ft_copy_pars_to_cmd(t_parser *pars)
{
	t_command *command;

	if (!pars)
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->value = pars->str;
	command->args = pars->args;
	command->type = pars->type;
	command->next = NULL;
	return (command);
}

void	ft_replace_our_struct(t_parser *pars)
{
	t_command *tmp;

	if (!pars)
		return ;
	data->command = ft_copy_pars_to_cmd(pars);
	if (!data->command)
		return ;
	pars = pars->next;
	while (pars)
	{
		tmp = ft_copy_pars_to_cmd(pars);
		if (!tmp)
			return ;
		ft_add_back_cmd(tmp);
		pars = pars->next;
	}
}

void    ft_final_result(char *line)
{
    t_tools* tools = malloc(sizeof(t_tools));

    tools->cmd = NULL;  // Initialize cmd to NULL
    t_token* token = malloc(sizeof(t_token));
    token->input = formate_line(line);
    add_history(token->input);
    tools->cmd = get_tokens(token->input);
	line_args(tools);
	// synatx_err(tools, token);

    // Print tokens
    t_token* current_token = tools->cmd;
    while (current_token != NULL) {
        printf("%s [%d]\n", current_token->input, current_token->type);
        current_token = current_token->next;
    }
	// char	**temp;

	// temp = line_tab(tools->cmd);
	// int i = 0;
	// printf("\nLines:\n");
    // while (temp[i])
	// {
    //     printf("%s\n", temp[i]);
    //     i++;
    // }

	t_parser *pars;

	pars = fill_parse_struct(tools);
	ft_replace_our_struct(pars);
	ft_heredoc();

    free(token->input);
    free(token);
    free(tools->cmd);
    free(tools);

}

void	ft_init_minishell(int ac, char **av, char **env)
{
    if (ac != 1)
	{
        ft_free_all("error in pass more args\n", 1);
	}
	data = malloc(sizeof(t_data));  // i need to use calloc instead of malloc 
	if (!data)
		ft_free_all("error in alloc data\n", 1);
	ft_bzero(data, sizeof(t_data));
	data->ac = ac;
	data->av = av;
	data->old_env = env;
	ft_create_new_env();
}

// =++++++++++++++++++++++++++++++  { part of printf commands } +++++++++++++++++++++++++++++++++++=

void print_type(int type)
{
	if (type == -1)
		printf("| type: ----------- ERROR   |\n+---------------------------+\n");
	else if (type == CMD)
		printf("| type: ----------- TOKEN   |\n+---------------------------+\n");
	else if (type == TRUNC)
		printf("| type: ----------- RED_OUT |\n+---------------------------+\n");
	else if (type ==INF)
		printf("| type: ----------- RED_IN  |\n+---------------------------+\n");
	else if (type == APPEND)
		printf("| type: ----------- APP     |\n+---------------------------+\n");
	else if (type == HEREDOC)
		printf("| type: ----------- HER_DOC |\n+---------------------------+\n");
	else if (type == PIPE)
		printf("| type: ----------- PIPE    |\n+---------------------------+\n\n");
	else if (type == ARG)
		printf("| type: ----------- ARG     |\n+---------------------------+\n\n");
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
	if (!table)
		return ;
	while (table != NULL)
	{
		printf("+---------------------------+\n");
		printf("| token: ---------- [%s]\n", table->value);
		print_type(table->type);
		print_args(table->args);
		table = table->next;
	}
}



int main(int ac, char** av, char** env)
{
	ft_init_minishell(ac, av, env);
	data->line = readline(ft_prompt());
	if (!data->line)
		return (0);
	ft_final_result(data->line);
	print_list(data->command);
	while (data->line)
	{
		ft_execution();
		ft_free_utils();
		data->line = readline(ft_prompt());
		if (data->line)
		{
			ft_final_result(data->line);
			print_list(data->command);
		}
	}
    ft_free_all(NULL, 0);
}