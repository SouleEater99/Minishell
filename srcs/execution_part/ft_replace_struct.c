#include "../../include/minishell.h"

void	ft_add_back_cmd(t_command *command)
{
	t_command *tmp;

	if (!data->command || !command)
		return ;
	tmp = data->command;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = command;
}


t_command *ft_copy_pars_to_cmd(t_parser *pars)
{
	t_command *command;

	if (!pars)
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->value = pars->str;
	command->args = pars->args;
	command->type = pars->type;
	command->next = NULL;
	return (command);
}

void	ft_replace_our_struct(t_parser *pars)
{
	t_command *tmp;

	if (!pars)
		return ;
	data->command = ft_copy_pars_to_cmd(pars);
	if (!data->command)
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

