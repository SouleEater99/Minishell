/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:33:06 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/28 22:58:06 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			free(tab[i++]);
		free(tab);
	}
}

void	ft_dup_error(t_pipe *ps, int fd1, int fd2)
{
	perror("dup error");
	close(fd1);
	if (fd2 != -1)
		close(fd2);
	ft_exit(ps, 1);
}

void	ft_exit(t_pipe *ps, int status)
{
	int		i;
	char	**tab;

	i = 0;
	close(ps->save_stdin);
	if (ps)
	{
		if (ps->cmd)
			free(ps->cmd);
		if (ps->path)
			free(ps->path);
		if (ps->arg)
		{
			tab = ps->arg;
			while (tab[i])
				free(tab[i++]);
			free(tab);
		}
		free(ps);
	}
	exit(status);
}

void	ft_print_error(char *cmd)
{
	int	size;

	size = ft_strlen(cmd);
	write(2, cmd, size);
	write(2, ": command not found\n", 21);
}
