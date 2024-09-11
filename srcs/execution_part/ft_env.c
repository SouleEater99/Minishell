/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:07:19 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:07:28 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*ft_lst_create_env_node(char *str)
{
	t_env	*new;

	if (!str)
		return (NULL);
	new = malloc(sizeof(t_env));
	if (!new)
		ft_free_all("Error in allocation of new_node in new_env\n", 1);
	new->value = ft_strdup(str);
	if (!new->value)
	{
		free(new);
		ft_free_all("Error in allocation in new_value\n", 1);
	}
	new->next = NULL;
	return (new);
}

void	ft_lst_add_back_env_node(t_env *next)
{
	t_env	*head;

	if (next)
	{
		head = g_data->new_env;
		while (head->next)
			head = head->next;
		head->next = next;
	}
}

void	ft_create_minimal_env(void)
{
	t_env	*next;
	char	*pwd;
	char	*tmp;

	g_data->new_env = ft_lst_create_env_node("SHLVL=1");
	// next = ft_lst_create_env_node("PATH=/bin:/usr/local/bin:/usr/bin:/sbin");
	// ft_lst_add_back_env_node(next);
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		tmp = ft_strjoin("PWD=", pwd);
		next = ft_lst_create_env_node(tmp);
		ft_lst_add_back_env_node(next);
		free(pwd);
		free(tmp);
	}
}

void	ft_create_new_env(void)
{
	int		i;
	char	*tmp;
	t_env	*next;

	i = 0;
	if (!g_data->old_env || !g_data->old_env[0])
	{
		ft_create_minimal_env();
		return ;
	}
	g_data->new_env = ft_lst_create_env_node(g_data->old_env[i++]);
	while (g_data->old_env[i])
	{
		if (ft_check_env_var(g_data->old_env[i], "SHLVL") == 0)
		{
			tmp = ft_update_shlvl(g_data->old_env[i] + 6);
			next = ft_lst_create_env_node(tmp);
			free(tmp);
		}
		else
			next = ft_lst_create_env_node(g_data->old_env[i]);
		ft_lst_add_back_env_node(next);
		i++;
	}
}

int		ft_check_equal_env(char *value)
{
	if (!value) 
		return (-1);
	while (*value)
		if (*value++ == '=')
			return (1);
	return (0);
}

void	ft_env(void)
{
	t_env	*env;

	env = g_data->new_env;
	while (env)
	{
		if (ft_check_equal_env(env->value) == 1)
			printf("%s\n", env->value);
		env = env->next;
	}
	ft_free_all(NULL, 0);
}
