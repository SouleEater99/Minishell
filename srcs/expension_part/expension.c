/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:13:55 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/12 10:57:09 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*apend_char_str(char *str, char c)
{
	int		i;
	char	*new;

	i = 0;
	if (str == NULL)
	{
		new = malloc(2);
		if (!new)
			return (NULL);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	i = ft_strlen(str);
	new = malloc(i + 2);
	if (!new)
		return (NULL);
	i = -1;
	while (str[++i])
		new[i] = str[i];
	new[i++] = c;
	new[i] = '\0';
	free(str);
	return (new);
}

static void	handle_dollar_sign(char *str, int flag, int *i, t_var *var)
{
	t_process_dollar	pd;

	pd.x = 0;
	if (str[*i + 1] == '\0')
		var->newstr = apend_char_str(var->newstr, str[(*i)++]);
	else if (str[*i + 1] == '?')
	{
		pd.exit_status_str = ft_itoa(g_data->exit);
		while (pd.exit_status_str[pd.x])
			var->newstr = apend_char_str(var->newstr,
					pd.exit_status_str[pd.x++]);
		free(pd.exit_status_str);
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

static void	process_char(char *str, int flag, int *i, t_var *var)
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

char	*expand_str(char *str, int flag)
{
	int		i;
	t_var	var;

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

void	expension(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->prev && (cur->type == FILENAME && cur->prev->type == HEREDOC))
			;
		else if (cur->type == FILENAME)
		{
			cur->input = expand_str(cur->input, 1);
			if (cur->prev && cur->prev->type != HEREDOC && cur->input
				&& cur->input[0] == '\0')
			{
				free(cur->input);
				cur->input = NULL;
			}
		}
		else
			cur->input = expand_str(cur->input, 1);
		cur = cur->next;
	}
}
