/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:09:03 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:09:05 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_add_back_cmd(t_command *command)
{
	t_command	*tmp;

	if (!g_data->command || !command)
		return ;
	tmp = g_data->command;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = command;
}

t_command	*ft_copy_pars_to_cmd(t_parser *pars)
{
	t_command	*command;

	if (!pars)
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->value = pars->str;
	command->args = pars->args;
	command->type = pars->type;
	command->index = pars->index;
	command->next = NULL;
	return (command);
}

void	ft_replace_our_struct(t_parser *pars)
{
	t_command	*tmp;

	if (!pars)
		return ;
	g_data->command = ft_copy_pars_to_cmd(pars);
	if (!g_data->command)
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
