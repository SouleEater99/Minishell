/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziz <aziz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 09:18:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/05/31 06:50:55 by aziz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
#define TYPES_H

# define MAX_ARGS 	15

typedef struct s_exec
{
	int			type;
	char		*argv[MAX_ARGS];
	char		*eargv[MAX_ARGS];
}				t_exec;

typedef struct s_redirec
{
	int			type;
	t_command	*cmd;
	char		*file;
	char		*efile;
	int			mode;
	int			fd;
}				t_redirec;

typedef struct s_pipe
{
	int			type;
	t_command	*left;
	t_command	*right;

}				t_pipe;

typedef struct s_listcmd
{
	int			type;
	t_command	*left;
	t_command	*right;

}				t_listcmd;

#endif