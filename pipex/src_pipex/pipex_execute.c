/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:34:02 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/28 22:36:06 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_first_cmd(t_pipe *ps, int *pip, char **envp)
{
	int	fd1;

	close(pip[0]);
	fd1 = open(ps->file1, O_RDONLY);
	if (fd1 == -1)
	{
		write(2, ps->file1, ft_strlen(ps->file1));
		perror(" ");
		close(pip[1]);
		ft_exit(ps, 1);
	}
	ft_assign(ps->av[ps->i], envp, ps);
	if (dup2(fd1, STDIN_FILENO) == -1)
		ft_dup_error(ps, fd1, pip[1]);
	close(fd1);
	if (dup2(pip[1], STDOUT_FILENO) == -1)
		ft_dup_error(ps, pip[1], -1);
	close(pip[1]);
	execve(ps->path, ps->arg, envp);
	perror("error in execve first cmd");
	ft_exit(ps, 1);
}

void	ft_middle_cmd(t_pipe *ps, int *pip, char **envp)
{
	ft_assign(ps->av[ps->i], envp, ps);
	close(pip[0]);
	if (dup2(pip[1], STDOUT_FILENO) == -1)
		ft_dup_error(ps, pip[1], -1);
	close(pip[1]);
	execve(ps->path, ps->arg, envp);
	perror("error in execve middle cmd");
	ft_exit(ps, 1);
}

void	ft_last_cmd(t_pipe *ps, int *pip, char **envp)
{
	int	fd2;

	ft_assign(ps->av[ps->i], envp, ps);
	close(pip[0]);
	close(pip[1]);
	if (!ps->limiter)
		fd2 = open(ps->file2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd2 = open(ps->file2, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd2 == -1)
	{
		write(2, ps->file2, ft_strlen(ps->file2));
		perror(" ");
		ft_exit(ps, 1);
	}
	if (dup2(fd2, STDOUT_FILENO) == -1)
		ft_dup_error(ps, fd2, -1);
	execve(ps->path, ps->arg, envp);
	perror("error in execve last cmd");
	ft_exit(ps, 1);
}

void	ft_child_proc(char **envp, t_pipe *ps, int *pip)
{
	close(ps->save_stdin);
	if (ps->i == 2)
		ft_first_cmd(ps, pip, envp);
	else if (ps->i == ps->ac - 2)
		ft_last_cmd(ps, pip, envp);
	else
		ft_middle_cmd(ps, pip, envp);
}

int	ft_execute(int ac, char **envp, t_pipe *ps)
{
	int	status;
	int	pip[2];

	while (ps->i < ac - 1)
	{
		if (pipe(pip) == -1)
			ft_exit(ps, 1);
		ps->pid = fork();
		if (ps->pid == -1)
			ft_exit(ps, 1);
		if (ps->pid == 0)
			ft_child_proc(envp, ps, pip);
		close(pip[1]);
		if (dup2(pip[0], STDIN_FILENO) == -1)
			ft_dup_error(ps, pip[0], -1);
		close(pip[0]);
		ps->i++;
	}
	waitpid(ps->pid, &status, 0);
	while (ps->i != -1)
		ps->i = wait(NULL);
	return (ft_wexitstatus(status));
}
