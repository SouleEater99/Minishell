/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/05/28 14:21:24 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

// void modify_env_var(char *var, )
// {
	
// }


// int join_str(char **env, char **cmd)
// {
// 	int len;
// 	char **new_env;
	
// 	len = -1;
// 	while(env[++len] != NULL);
// 	new_env = malloc((len + 1) * sizeof(char *));
// 	if (!new_env)
// 		return (0);
// 	len = -1;
// 	while(env[++len])
// 	{
// 		// new_env[len] = malloc((env[len] + 1) * sizeof(char));
// 		new_env[len] = ft_strdup(env[len]);
// 	}
// 	new_env[len] = ft_strdup(cmd[1]);
// 	new_env[++len] = NULL;
// 	return (1);
// }

int	export(char *cmd)
{
	// int	i;

	// i = 0;

	printf("%s\n", cmd);
	// if (cmd[1] == NULL)
	// {
	// 	env(data->env);
	// 	return (0);
	// }
	// while (env[++i] != NULL)
	// {
		// if (ft_strncmp(exprt_var, env[i], ft_strlen(env[i])) == 0)
		// 	modify_env_var(env);
	// }
	// join_str(data->env, cmd);
	// printf("%s\n", env[i]);
	return (0);
}

int unset()
{
    return (0);
}