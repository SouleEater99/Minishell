/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:08:45 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:08:47 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_type(int type)
{
	if (type == -1)
		printf("| type: ----------- ERROR   |\n+----------------+\n");
	else if (type == CMD)
		printf("| type: ----------- TOKEN   |\n+----------------+\n");
	else if (type == TRUNC)
		printf("| type: ----------- RED_OUT |\n+----------------+\n");
	else if (type == INF)
		printf("| type: ----------- RED_IN  |\n+----------------+\n");
	else if (type == APPEND)
		printf("| type: ----------- APP     |\n+----------------+\n");
	else if (type == HEREDOC)
		printf("| type: ----------- HER_DOC |\n+----------------+\n");
	else if (type == PIPE)
		printf("| type: ----------- PIPE    |\n+--------------+\n\n");
	else if (type == ARG)
		printf("| type: ----------- ARG     |\n+--------------+\n\n");
}

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args != NULL && *args)
	{
		printf("	+---------------------------+\n");
		printf("	| arg[%d]: --------- [%s]\n", i++, *args++);
		printf("	+---------------------------+\n");
	}
}

void	print_list(t_command *table)
{
	if (!table)
		return ;
	while (table != NULL)
	{
		printf("+---------------------------+\n");
		printf("| token: ---------- [%s]\n", table->value);
		print_type(table->type);
		print_args(table->args);
		table = table->next;
	}
}

void	exit_fork(int status)
{
	int	pid;
	int	status1;

	pid = fork();
	if (pid == 0)
		exit(status);
	wait(&status);
	printf("+++++++++++++ { before => status : %d } ++++++++++++++++\n",
		status);
	status1 = WEXITSTATUS(status);
	printf("+++++++++++++ {  weexitstatus : %d } ++++++++++++++++\n", status1);
	status1 = WIFEXITED(status);
	printf("+++++++++++++ {  wifexed: %d } ++++++++++++++++\n", status1);
	status1 = WIFSIGNALED(status);
	printf("+++++++++++++ {  wifsgnaled : %d } ++++++++++++++++\n", status1);
	printf("+++++++++++ { HEREDOC } +++++++++++++++++++++++\n");
}
