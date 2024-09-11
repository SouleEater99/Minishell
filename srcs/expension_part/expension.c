/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:13:55 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 22:32:40 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char    *apend_char_str(char *str, char c)
{
    int        i;
    char    *new;

    i = 0;
    if (str == NULL)
    {
        if (!(new = malloc(2)))
            return (NULL);
        new[0] = c;
        new[1] = '\0';
        return (new);
    }
    i = ft_strlen(str);
    if (!(new = malloc(i + 2)))
        return (NULL);
    i = 0;
    while (str[i])
    {
        new[i] = str[i];
        i++;
    }
    new[i++] = c;
    new[i] = '\0';
    free(str);
    return (new);
}

char	*ft_appand(char *var, char *newstr)
{
	int		j;
	char	*path;
    
	j = -1;
	path = ft_expand_var(var);
	free(var);
	while (path && path[++j])
		 newstr = apend_char_str(newstr, path[j]);
	return (newstr);
}

static void handle_dollar_sign(char *str,int flag, int *i, t_var *var)
{
    int x;
    char *exit_status_str;

    x = 0;
    if (str[*i + 1] == '\0')
        var->newstr = apend_char_str(var->newstr, str[(*i)++]);
    else if (str[*i + 1] == '?')
    {
        exit_status_str = ft_itoa(g_data->exit);
        while (exit_status_str[x])
            var->var = apend_char_str(var->var, exit_status_str[x++]);
        var->newstr = var->var;
        free(exit_status_str);
        *i += 2;
    }
    else if ((str[*i + 1] == '\'' || str[*i + 1] == '\"') && flag == 1)
        (*i)++;
    else if (str[*i + 1] == '_' || ft_isalpha(str[*i + 1]))
    {
        (*i)++;
        while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
            var->var = apend_char_str(var->var, str[(*i)++]);
        var->newstr = ft_appand(var->var, var->newstr);
    }
    else
        var->newstr = apend_char_str(var->newstr, str[(*i)++]);
}

static void process_char(char *str,int flag ,int *i, t_var *var)
{
    if (str[*i] == '\'' && var->dq == 1 && flag == 1)
        var->sq *= (-1);
    else if (str[*i] == '\"' && var->sq == 1)
        var->dq *= (-1);
    
    if (str[*i] == '$' && var->sq == 1)
        handle_dollar_sign(str, flag, i, var);
    else if (str[*i] == '$' && ft_isdigit(str[*i + 1]))
        *i += 2;
    else
        var->newstr = apend_char_str(var->newstr, str[(*i)++]);
}

char *expand_str(char *str,int flag)
{
    int i;
    t_var var;

    var.sq = 1;
    var.dq = 1;
    var.newstr = NULL;
    var.var = NULL;
    i = 0;

    if (!str)
        return (NULL);

    while (str[i])
    {
        var.var = NULL;
        process_char(str, flag, &i, &var);
    }

    if (!var.newstr)
        var.newstr = ft_calloc(1, sizeof(char *));

    free(str);
    return (var.newstr);
}

static void process_token(t_token **token, t_token **cur, t_token **prev, t_token *next)
{
    if (((*cur)->type == CMD || (*cur)->type == ARG) && (*cur)->input[0] == '\0')
    {
        if (*prev)
            (*prev)->next = next;
        else
            *token = next;

        if (next)
            next->prev = *prev;
        free((*cur)->input);
        free(*cur);
        *cur = next;
    }
    else
    {
        *prev = *cur;
        *cur = next;
    }
}

void ignore_empty_cmd_arg(t_token **token)
{
    t_token *cur;
    t_token *next;
    t_token *prev;
    
    if (!token || !*token)
        return;

    cur = *token;
    prev = NULL;

    while (cur)
    {
        next = cur->next;
        process_token(token, &cur, &prev, next);
    }
    get_tokens_type(*token, 0);
}

void    expension(t_token *token)
{
    t_token *cur;
    
    cur = token;
    while (cur)
    {
        if (cur->prev && (cur->type == FILENAME && cur->prev->type == HEREDOC))
            ;
        else
            cur->input = expand_str(cur->input, 1);
        cur = cur->next;
    }
    
}

int     heredoc_quotes(char *line)
{
    int i;
    int open;

    i = 0;
    open = 0;
    while (line[i])
    {
        if (i > 0 && line[i - 1] == '\\')
            i++;
        else if (open == 0 && line[i] == '\"')
            open = 1;
        else if (open == 0 && line[i] == '\'')
            open = 2;
        else if (open == 1 && line[i] == '\"')
            open = 4;
        else if (open == 2 && line[i] == '\'')
            open = 6;
        i++;
    }
    return (open);
}

void    expand_flag(t_parser *parser)
{
    t_parser *cur;
    int quote_status;

    cur = parser;
    
    while (cur)
    {
        if (cur && cur->type == HEREDOC)
        {
            quote_status = heredoc_quotes(cur->args[0]);
            
            if (quote_status == 4 || quote_status == 6)
            {
                cur->index = 1;
            }
            else if(quote_status == 1 || quote_status == 2)
            {
                //syntax error open quotes
            }
        }
        cur = cur->next;
    }
}
