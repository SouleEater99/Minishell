#include "../include/minishell.h"

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
	else if (ft_strcmp(token->input, "|") == 0 && sep == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->input, ";") == 0 && sep == 0)
		token->type = END;
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

int		is_sep(char *line, int i)
{
	if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
		return (0);
	else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
		return (1);
	else
		return (0);
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
			new[j++] = (char)(-line[i++]);
		else if (quotes(line, i) == 0 && is_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
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
		if (!prev || is_type(prev, END) || is_type(prev, PIPE))
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
	else if (ft_strchr(types, 'E') && is_type(token, END))
		return (1);
	return (0);
}

// int main(int ac, char** av, char** env)
// {

//     t_tools* tools = malloc(sizeof(t_tools));
// 	char *tmp;

//     tools->cmd = NULL;  // Initialize cmd to NULL

//     if (ac != 1 || av[1]) {
//         printf("This program does not accept arguments\n");
//         exit(0);
//     }
//     t_token* token = malloc(sizeof(t_token));

//     char *s = readline("mini_shell$ ");
// 	token->input = formate_line(s);
// 	free(s);
//     add_history(token->input);
//     tools->cmd = get_tokens(token->input);
// 	// squish_args(tools);

//     // Print tokens
//     t_token* current_token = tools->cmd;
//     while (current_token != NULL) {
//         printf("\n%s [%d]\n", current_token->input, current_token->type);
//         current_token = current_token->next;
//     }

//     // Free memory
//     free(token->input);
//     free(token);
//     free(tools->cmd);
//     free(tools);

// }