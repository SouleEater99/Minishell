/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:37:51 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:37:53 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_check_export_arg(char *str)
{
	int	i;

	i = -1;
	if (!str[0] || str[0] == '=' || !((str[0] >= 'A' && str[0] <= 'Z')
			|| (str[0] >= 'a' && str[0] <= 'z')))
		if (str[0] != '_')
			return (0);
	while (str[++i])
	{
		if (ft_check_export_plus(str, i) == 0)
			return (0);
		else if (ft_check_export_plus(str, i) == 2)
			return (2);
		if (str[i] == '=')
			return (1);
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '-')
			return (0);
		if (str[i] != '_' && !((str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= 'a' && str[i] <= 'z')))
			if ((str[i] < '0' && str[i] > '9'))
				return (0);
	}
	return (3);
}

char	*ft_get_identifier(char *var)
{
	int		i;
	char	*new;

	i = 0;
	while (var[i] && var[i] != '=' && var[i] != '+')
		i++;
	new = malloc(i + 1);
	if (!new)
		ft_free_all("Error in allocation of identifier of export\n", 1);
	i = 0;
	while (var[i] && var[i] != '=' && var[i] != '+')
	{
		new[i] = var[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_replace_and_join(char *value, char *var)
{
	char	*new;

	new = NULL;
	if (ft_check_export_arg(var) == 1 || ft_check_export_arg(var) == 3)
	{
		free(value);
		new = ft_strdup(var);
	}
	else if (ft_check_export_arg(var) == 2)
	{
		while (*var)
			if (*var++ == '=')
				break ;
		new = ft_strjoin(value, var);
		free(value);
	}
	return (new);
}

char	*ft_update_identifier(char *identifier, char *var)
{
	char	*new;

	if (!identifier || !var)
		return (free(identifier), NULL);
	while (*var && *var != '=')
		var++;
	new = ft_strjoin(identifier, var);
	free(identifier);
	return (new);
}

int	ft_add_or_update(char *var)
{
	t_env	*head;
	char	*identifier;

	if (!var)
		return (0);
	identifier = ft_get_identifier(var);
	head = g_data->new_env;
	while (head)
	{
		if (ft_check_env_var(head->value, identifier) == 0)
		{
			free(identifier);
			head->value = ft_replace_and_join(head->value, var);
			if (!head->value)
				ft_free_all("error in allocation export\n", 1);
			return (0);
		}
		head = head->next;
	}
	identifier = ft_update_identifier(identifier, var);
	if (g_data->new_env)
		head = ft_lst_create_env_node(identifier);
	else
		g_data->new_env = ft_lst_create_env_node(identifier);
	return (free(identifier), ft_lst_add_back_env_node(head), 0);
}
