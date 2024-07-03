/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 22:24:16 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/29 00:22:34 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipe	*ps;
	int		i;

	i = 0;
	ps = NULL;
	if (ac == 5)
	{
		ps = ft_init_ps(ac, av, ps);
		ps->i = 2;
		i = ft_execute(ac, envp, ps);
		dup2(ps->save_stdin, STDIN_FILENO);
		close(ps->save_stdin);
		free(ps);
	}
	return (i);
}
