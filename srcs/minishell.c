/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:56 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 22:33:30 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_data *g_data = NULL;

// void print_parser(t_parser *pars)
// {
// 	t_parser *current_parser;
	
// 	current_parser =  pars;
//     while (current_parser)
//     {
//         printf("str: %s type: %d args: || addr %p", current_parser->str, current_parser->type, current_parser->str);
//         if (current_parser->args)
//         {
//             char **arg = current_parser->args;
//             while (*arg)
//             {
//                 printf("\"%s || %p\" ", *arg, *arg);
//                 arg++;
//             }
//         }
//         else
//         {
//             printf("empty ");
//         }
//         printf(" --> ");
        
//         current_parser = current_parser->next;
//     }
//     printf("NULL\n");
// }

// +++++++++++++++++++++++++++++ { from here i started to merge} ++++++++++++++++++++++++++++++++++++

void    ft_final_result(char *line)
{
    t_tools* tools = malloc(sizeof(t_tools));
	char	*input;
	int x = 0;
	// char	**args;
	// t_token *pipe = NULL;
	
    tools->cmd = NULL;  // Initialize cmd to NULL
    // t_token* token = malloc(sizeof(t_token));
	
	if (line && *line)
    	add_history(line);
    input = formate_line(line);
    tools->cmd = get_tokens(input);
	get_tokens_type(tools->cmd, 0);
	expension(tools->cmd);
	// args = line_tab(tools->cmd);
	// line_args(tools);
	g_data->syn_err = synatx_err(tools);
    // Print tokens
	ignore_empty_cmd_arg(&tools->cmd);
	split_token_input(tools->cmd);
    t_token* current_token = tools->cmd;
    while (current_token != NULL)
	{
		x = 0;
        printf("{%s} [%d]", current_token->input, current_token->type);
		printf(" / ARGS : ");
		while (current_token->args && current_token->args[x])
		{
			printf(" [%s]", current_token->args[x++]);
		}
		printf("\n");
        current_token = current_token->next;
    }
	g_data->command = NULL;
	t_parser *pars;
	
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
	free_token_list(tools->cmd);
    free(tools);
	ft_heredoc();

}

// =++++++++++++++++++++++++++++++  { part of printf commands } +++++++++++++++++++++++++++++++++++=

int main(int ac, char** av, char** env)
{
	ft_init_minishell(ac, av, env);
	g_data->line = readline(ft_prompt(1));
	if (!g_data->line)
		ft_free_all(NULL, g_data->exit);
	signal(SIGINT, SIG_IGN);
	ft_final_result(g_data->line);
	print_list(g_data->command);
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
			print_list(g_data->command);
		}
	}
	ft_free_all(NULL, g_data->exit);
}