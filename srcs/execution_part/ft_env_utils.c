/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 00:08:44 by ael-maim          #+#    #+#             */
/*   Updated: 2024/09/10 00:08:47 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_calc_env(void)
{
	int		i;
	t_env	*next;

	i = 0;
	next = g_data->new_env;
	while (next)
	{
		next = next->next;
		i++;
	}
	return (i);
}

char	*ft_update_shlvl(char *str)
{
	int		num;
	char	*tmp;
	char	*tmp2;

	if (!str)
		return (NULL);
	num = ft_atoi(str);
	num++;
	tmp = ft_itoa(num);
	if (!tmp)
		return (NULL);
	tmp2 = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	return (tmp2);
}

int	ft_check_equal_env(char *value)
{
	if (!value)
		return (-1);
	while (*value)
		if (*value++ == '=')
			return (1);
	return (0);
}

char	*ft_get_env_last_cmd(t_command *last)
{
	char	*line;
	char	**tab;
	int		i;

	i = 0;
	tab = last->args;
	if (!tab)
		return (NULL);
	while (tab[i] && tab[i + 1])
		i++;
	line = ft_strjoin("_=", tab[i]);
	return (line);
}

void	ft_update_env_last_cmd(void)
{
	t_command	*cmd;
	t_command	*last_cmd;
	char		*line;

	cmd = g_data->command;
	last_cmd = NULL;
	if (!cmd)
		return ;
	while (cmd)
	{
		if (cmd->type == CMD)
			last_cmd = cmd;
		cmd = cmd->next;
	}
	if (last_cmd)
	{
		line = ft_get_env_last_cmd(last_cmd);
		ft_add_or_update(line);
		if (line)
			free(line);
	}
}
