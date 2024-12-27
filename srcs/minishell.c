/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:56 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 12:08:11 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_data		*g_data = NULL;

static void	token_parse(t_tools *tools, char *input)
{
	tools->cmd = get_tokens(input);
	get_tokens_type(tools->cmd, 0);
	expension(tools->cmd);
	g_data->syn_err = synatx_err(tools);
	ignore_empty_cmd_arg(&tools->cmd);
	split_token_input(tools->cmd);
	ambigeous_file(tools->cmd);
}

static void	cp_parse(t_tools *tools, t_parser *pars)
{
	g_data->command = NULL;
	if (g_data->syn_err == 1)
	{
		pars = fill_parse_struct(tools);
		expand_flag(pars);
		del_q(pars);
		ft_replace_our_struct(pars);
		free_parser_list(pars);
	}
	else
	{
		g_data->exit = 2;
		g_data->command = NULL;
	}
}

void	ft_final_result(char *line)
{
	t_tools		*tools;
	char		*input;
	t_parser	*pars;

	tools = malloc(sizeof(t_tools));
	pars = NULL;
	tools->cmd = NULL;
	if (line && *line)
		add_history(line);
	input = formate_line(line);
	token_parse(tools, input);
	cp_parse(tools, pars);
	free_token_list(tools->cmd);
	free(tools);
	ft_heredoc();
}

int	main(int ac, char **av, char **env)
{
	ft_init_minishell(ac, av, env);
	g_data->line = readline(ft_prompt(1));
	if (!g_data->line)
		ft_free_all(NULL, g_data->exit);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	ft_final_result(g_data->line);
	while (g_data->line)
	{
		ft_execution();
		signal(SIGINT, ft_sig_handler);
		ft_free_utils();
		g_data->line = readline(ft_prompt(2));
		if (g_data->line)
		{
			signal(SIGINT, SIG_IGN);
			ft_final_result(g_data->line);
		}
	}
	ft_free_all(NULL, g_data->exit);
}
