/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:14:07 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:14:11 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_utils_2(void)
{
	if (g_data->exec_env)
	{
		ft_free_tab(g_data->exec_env);
		g_data->exec_env = NULL;
	}
	if (g_data->prompt)
	{
		free(g_data->prompt);
		g_data->prompt = NULL;
	}
	if (g_data->command)
	{
		ft_free_command(g_data->command);
		g_data->command = NULL;
	}
}

void	ft_free_utils(void)
{
	if (g_data)
	{
		g_data->i = 0;
		if (g_data->pip)
			ft_close_free_heredoc_pipes();
		if (g_data->path)
		{
			free(g_data->path);
			g_data->path = NULL;
		}
		if (g_data->tab)
		{
			ft_free_tab(g_data->tab);
			g_data->tab = NULL;
		}
		ft_free_utils_2();
	}
}
