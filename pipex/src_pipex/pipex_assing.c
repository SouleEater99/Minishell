/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_assing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:36:31 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/28 23:16:56 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*ft_strdup_path(char *str)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '	')
		i++;
	dup = (char *)malloc(i + 1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '	')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	ft_check_sanitize(t_pipe *ps)
{
	int		i;

	i = 0;
	while (ps->path[i] && ps->path[i] != ' ' && ps->path[i] != '	')
	{
		if (ps->path[i] == '\\' && ps->path[i + 1] && ps->path[i + 1] == '\\')
		{
			ft_putstr_fd("no such file or directory\n", 2);
			ft_exit(ps, 127);
		}
		if (ps->path[i] == '\\' && ps->path[i + 1] && ps->path[i + 1] != '\\')
			ps->path[i] = '/';
		i++;
	}
	if (access(ps->path, F_OK) != 0)
	{
		perror(ps->cmd);
		ft_exit(ps, 127);
	}
	if (access(ps->path, X_OK) != 0)
	{
		perror(ps->cmd);
		ft_exit(ps, 126);
	}
}

void	ft_assign(char *cmd, char **envp, t_pipe *ps)
{
	ps->cmd = ft_get_cmd(cmd, ps);
	ps->path = ft_get_path(envp, cmd, ps);
	ps->arg = ft_split(cmd, ' ');
	if (!ps->path)
	{
		ft_print_error(ps->cmd);
		ft_exit(ps, 127);
	}
	ft_check_sanitize(ps);
}

t_pipe	*ft_init_ps(int ac, char **av, t_pipe *ps)
{
	ps = malloc(sizeof(t_pipe));
	if (!ps)
		exit(1);
	ps->path = NULL;
	ps->cmd = NULL;
	ps->arg = NULL;
	ps->limiter = NULL;
	ps->file1 = av[1];
	ps->file2 = av[ac - 1];
	ps->av = av;
	ps->ac = ac;
	ps->save_stdin = dup(STDIN_FILENO);
	if (ps->save_stdin == -1)
		ft_exit(ps, 1);
	return (ps);
}

int	ft_wexitstatus(int status)
{
	status = status >> 8;
	status = status & 255;
	return (status);
}
