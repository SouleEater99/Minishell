/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:52:55 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/05 15:50:40 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*ft_appand(char *var, char *newstr,t_env *envp)
{
	int		j;
	char	*path;
    
    (void)envp;
	j = -1;
	path = ft_expand_var(var);
	free(var);
	while (path && path[++j])
		 newstr = apend_char_str(newstr, path[j]);
	return (newstr);
}

char	*add_to_str(char *str, t_var var, int *i)
{
	if (str[*i] == '$' && var.sq == 1 && var.dq == 1 && (str[*i
			+ 1] == '\"' || str[*i + 1] == '\''))
		*i += 1;
	else
	{
		var.newstr = apend_char_str(var.newstr, str[*i]);
		*i += 1;
	}
	return (var.newstr);
}
char	*expand_str(char *str, t_env *envp)
{
    int		i;
    t_var	var;

    i = 0;
    var.sq = 1;
    var.dq = 1;
    var.newstr = NULL;

    while (str[i])
    {
        var.var = NULL;
        if (str[i] == '\'' && var.dq == 1)
            var.sq *= (-1);
        else if (str[i] == '\"' && var.sq == 1)
            var.dq *= (-1);
        if (str[i] == '$' && str[i + 1] == '\0')
            var.newstr = apend_char_str(var.newstr, str[i++]);
        else if (str[i] == '$' && var.sq == 1)
        {
            if (str[i + 1] == '_')
            {
                var.var = apend_char_str(var.var, str[i + 1]);
                i += 2;
            }
            // else if (str[i + 1] == '?')
            // {
            //     var.var = ft_itoa(data->exit);
            //     i += 2;
            // }
            else if (ft_isalpha(str[i + 1]))
            {
                while (str[++i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    var.var = apend_char_str(var.var, str[i]);
            }
            else
            {
                var.newstr = apend_char_str(var.newstr, str[i++]);
                continue;
            }
            var.newstr = ft_appand(var.var, var.newstr, envp);
        }
        else if (str[i] == '$' && ft_isdigit(str[i + 1]))
            i += 2;
        else if (str[i] != '\0')
            var.newstr = add_to_str(str, var, &i);
    }
    free (str);
    return (var.newstr);
}

char	*rm_quotes(char *str)
{
	int		i;
	int		sinqot;
	int		dobqot;
	char	*newstr;

	i = 0;
	sinqot = 1;
	dobqot = 1;
	newstr = NULL;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && dobqot == 1)
			sinqot *= (-1);
		if (str[i] == '\"' && sinqot == 1)
			dobqot *= (-1);
		if (str[i] == '\'' && dobqot == 1)
			i++;
		else if (str[i] == '\"' && sinqot == 1)
			i++;
		else
		{
			newstr = apend_char_str(newstr, str[i]);
			i++;
		}
	}
	free(str);
	return (newstr);
}

void expension(t_parser *parser, t_tools *tools)
{
    t_parser *cur;
    int i;

    cur = parser;

    while (cur)
    {
		if (cur->type == HEREDOC)
			cur = cur->next;
        if (cur && cur->str)
        {
            cur->str = expand_str(cur->str, tools->env);
            cur->str = rm_quotes(cur->str);
        }

        if (cur && cur->args)
        {
            i = 0;
            while (cur->args[i])
            {
                cur->args[i] = expand_str(cur->args[i], tools->env);
                cur->args[i] = rm_quotes(cur->args[i]);
                i++;
            }
        }
        cur = cur->next;
	}
}