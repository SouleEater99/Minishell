/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:06:59 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:07:03 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_switch(char *str, char *switches)
{
	int i;
	int	j;

	i = 0;
	if (!str || !switches)
		return (0);
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		j = 0;
		while (switches[j])
			if (switches[j++] != str[i])
				return (-1);
		i++;
	}
	return (1);
}

int	ft_echo(char **arg)
{
	int	nl_flag;
	int	i;
	int	number_of_arg;

	i = 1;
	nl_flag = 0;
	if (!arg || !arg[1])
		return (write (1, "\n", 2), ft_free_all(NULL, 0), 0);
	number_of_arg = ft_tab_lenght(arg);
	if (arg[1] && ft_is_switch(arg[1], "n") == 1)
		nl_flag = 1;
	if (number_of_arg == 1 && nl_flag == 1)
		return (ft_free_all(NULL, 0), 0);
	if (nl_flag == 1)
		i = 2;
	while (arg[i])
	{
		ft_putstr_fd(arg[i], 1);
		if (i++ < number_of_arg - 1)
			write(1, " ", 2);
	}
	if (nl_flag == 0)
		write(1, "\n", 2);
	return (ft_free_all(NULL, 0), 0);
}

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		ft_free_all("error in allocation of pwd\n", 1);
	printf("%s\n", pwd);
	free(pwd);
	ft_free_all(NULL, 0);
}

void	ft_check_exit_arg(char **args)
{
	int i;

	i = 0;
	while (args[1][i])
	{
		if (args[1][i] < '0' || args[1][i] > '9')
		{
			ft_putstr_fd(args[1], 2);
			g_data->exit = 2;
			ft_free_all(": numeric argument required\n", 2);
		}
		i++;
	}
	if (i > 19)
	{
		ft_putstr_fd(args[1], 2);
		g_data->exit = 2;
		ft_free_all(": numeric argument required\n", 2);
	}
}

int	ft_exit(char **args)
{
	int				i;
	unsigned char	c;

	i = 0;
	if (!args || !args[1])
		ft_free_all(NULL, 0);
	ft_check_exit_arg(args);
	i = ft_tab_lenght(args);
	if (i > 2)
		return ((g_data->exit = 2),
			ft_putstr_fd("bash: exit: too many arguments\n", 2), 0);
	c = ft_atoi(args[1]);
	ft_free_all(NULL, c);
	return (0);
}
