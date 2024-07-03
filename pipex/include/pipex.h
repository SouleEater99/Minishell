/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:46:34 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/28 23:16:50 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../get_next_line/get_next_line.h"
# include "../lib_ft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

typedef struct t_pipe
{
	char	**arg;
	char	**av;
	char	*cmd;
	char	*path;
	char	*file1;
	char	*file2;
	char	*limiter;
	int		pid;
	int		i;
	int		ac;
	int		save_stdin;
}			t_pipe;

void		ft_first_cmd(t_pipe *ps, int *pip, char **envp);
void		ft_middle_cmd(t_pipe *ps, int *pip, char **envp);
void		ft_last_cmd(t_pipe *ps, int *pip, char **envp);
void		ft_child_proc(char **envp, t_pipe *ps, int *pip);
int			ft_execute(int ac, char **envp, t_pipe *ps);
char		*ft_get_cmd(char *cmd, t_pipe *ps);
size_t		ft_strlen_delimiter(const char *str);
char		*ft_strjoin_path(char const *s1, char const *s2, char c);
int			ft_check_path(char *cmd);
char		*ft_get_path(char **envp, char *cmd, t_pipe *ps);
void		ft_dup_error(t_pipe *ps, int fd1, int fd2);
void		ft_free_tab(char **tab);
void		ft_exit(t_pipe *ps, int status);
void		ft_print_error(char *cmd);
void		ft_assign(char *cmd, char **envp, t_pipe *ps);
t_pipe		*ft_init_ps(int ac, char **av, t_pipe *ps);
int			ft_wexitstatus(int status);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strdup_path(char *str);

#endif
