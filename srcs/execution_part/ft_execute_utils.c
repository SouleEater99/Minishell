#include "../../include/minishell.h"


int ft_numbers_of_cmd(t_command *cmd)
{
	int count;

	count = 0;
	while (cmd)
	{
		if (cmd->type == PIPE)
			count++;
		cmd = cmd->next;
	}
	return (count);
}



t_command *ft_return_next_cmd(t_command *cmd)
{
	while (cmd && cmd->type != PIPE)
		cmd = cmd->next;
	if (!cmd)
		return (cmd);
	return (cmd->next);
}

t_command *ft_return_cmd_index(t_command *cmd)
{
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == CMD)
			return (cmd);
		cmd = cmd->next;
	}
	return (NULL);
}

char *fetch_path_of_cmd(t_env *env, char *cmd)
{
	int i;

	i = 0;
	if (check_cmd_is_path(cmd) == 1)
		return (ft_check_path(data->path), ft_strdup(cmd));
	while (env && ft_strnstr(env->value, "PATH=", 6) == NULL)
		env = env->next;
	if (!env)
		return (ft_strjoin("./", cmd));
	data->tab = ft_split(env->value + 6, ':');
	if (!data->tab)
		ft_free_all("error in split in fetch path\n", 1);
	while (data->tab[i])
	{
		data->path = ft_strjoin_path(data->tab[i++], cmd, '/');
		if (access(data->path, F_OK) == 0)
			return (data->path);
		free(data->path);
	}
	return (NULL);
}
