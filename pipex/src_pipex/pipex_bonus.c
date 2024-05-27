/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:04:48 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/28 23:19:32 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_strcmp(char *s1, char *s2)
{
	if (s1 && s2)
	{
		while (*s1 && *s2 && *s1 == *s2)
		{
			s1++;
			s2++;
		}
	}
	else
		return (0);
	if (*s1 == '\n')
		s1++;
	return (*s1 - *s2);
}

void	ft_here_doc(char **av, t_pipe *ps)
{
	char	*line;
	int		pip[2];

	if (pipe(pip) == -1)
		ft_exit(ps, 1);
	ft_putstr_fd("Here_Doc Pipe --> ", 1);
	line = get_next_line(0);
	while (line && ft_strcmp(line, av[2]) != 0)
	{
		write(pip[1], line, ft_strlen(line));
		free(line);
		ft_putstr_fd("Here_Doc Pipe --> ", 1);
		line = get_next_line(0);
	}
	close(pip[1]);
	if (line)
		free(line);
	if (dup2(pip[0], STDIN_FILENO) == -1)
		ft_dup_error(ps, pip[0], -1);
	close(pip[0]);
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	*ps;
	int		i;

	i = 0;
	ps = NULL;
	if (ac >= 5)
	{
		if (ft_strcmp(av[1], "here_doc") != 0)
		{
			ps = ft_init_ps(ac, av, ps);
			ps->i = 2;
		}
		else
		{
			ps = ft_init_ps(ac, av, ps);
			ps->limiter = av[2];
			ps->i = 3;
			ft_here_doc(av, ps);
		}
		i = ft_execute(ac, envp, ps);
		dup2(ps->save_stdin, STDIN_FILENO);
		close(ps->save_stdin);
		free(ps);
	}
	return (i);
}
