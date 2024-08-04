/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:52:55 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/01 10:17:19 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_var
{
	int	sq;
	int	dq;
	char	*newstr;
	char *var;
}	t_var;



int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}
int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

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
char	*expand_str(char *str,t_env *envp)
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
		else if (str[i] == '$' && var.sq == 1 && ft_isalpha(str[i + 1]))
		{
			while (str[++i] && ft_isalnum(str[i]))
				var.var = apend_char_str(var.var, str[i]);
		}
		else if (str[i] == '$' && ft_isdigit(str[i + 1]))
			i += 2;
		else if (str[i] != '\0')
			var.newstr = add_to_str(str, var, &i);
		if (var.var != NULL)
			var.newstr = ft_appand(var.var, var.newstr,envp);
	}
	// free (str);
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
