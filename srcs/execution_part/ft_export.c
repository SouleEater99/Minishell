/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:08:04 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:08:06 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_get_env_in_tab(void)
{
	char	**tab;
	int		i;
	t_env	*next;

	i = ft_calc_env();
	tab = malloc(sizeof(char *) * (i + 1));
	next = g_data->new_env;
	i = 0;
	while (next)
	{
		tab[i++] = ft_strdup(next->value);
		next = next->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	ft_print_qoutes(char *str)
{
	write (1, "declare -x ", 12);
	while (*str)
	{
		write (1, str, 1);
		if (*str++ == '=')
			break;
	}
	if (*str)
	{
		ft_putchar_fd('"', 1);
		ft_putstr_fd(str, 1);
		ft_putchar_fd('"', 1);
	}
	ft_putstr_fd("\n", 1);
}

void	ft_print_export(void)
{
	char	**tab;
	char	*tmp;
	int		n;

	tab = ft_get_env_in_tab();
	g_data->i = -1;
	n = ft_calc_env();
	while (++g_data->i < n)
	{
		g_data->j = -1;
		while (tab[++g_data->j])
		{
			if (tab[g_data->j + 1] && ft_strcmp(tab[g_data->j], tab[g_data->j
						+ 1]) > 0)
			{
				tmp = tab[g_data->j];
				tab[g_data->j] = tab[g_data->j + 1];
				tab[g_data->j + 1] = tmp;
			}
		}
	}
	g_data->i = 0;
	while (tab[g_data->i])
		ft_print_qoutes(tab[g_data->i++]);
	ft_free_tab(tab);
}

int	ft_export(char **arg)
{
	static unsigned int	i;
	int					n;

	i++;
	if (arg)
		n = ft_tab_lenght(arg);
	if (!arg || !arg[1] || n == 1)
	{
		if (g_data->new_env)
			ft_print_export();
		return ((g_data->exit = 0), 0);
	}
	if (arg[i])
	{
		if (ft_check_export_arg(arg[i]) == 0)
		{
			ft_putstr_fd("export: has not a valid identifier\n", 2);
			g_data->exit = 1;
		}
		else if (ft_check_export_arg(arg[i]) > 0)
			ft_add_or_update(arg[i]);
		ft_export(arg);
	}
	i = 0;
	return ((g_data->exit = 0), 0);
}
