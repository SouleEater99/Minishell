/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:56 by samsaafi          #+#    #+#             */
/*   Updated: 2024/08/17 11:52:10 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_data *g_data = NULL;

void print_parser(t_parser *pars)
{
	t_parser *current_parser;
	
	current_parser = pars;
    while (current_parser)
    {
        printf("str: %s type: %d args: || addr %p", current_parser->str, current_parser->type, current_parser->str);
        if (current_parser->args)
        {
            char **arg = current_parser->args;
            while (*arg)
            {
                printf("\"%s || %p\" ", *arg, *arg);
                arg++;
            }
        }
        else
        {
            printf("empty ");
        }
        printf(" --> ");
        
        current_parser = current_parser->next;
    }
    printf("NULL\n");
}

// +++++++++++++++++++++++++++++ { from here i started to merge} ++++++++++++++++++++++++++++++++++++

void    ft_final_result(char *line)
{
    t_tools* tools = malloc(sizeof(t_tools));
	char	*input;
	
    tools->cmd = NULL;  // Initialize cmd to NULL
    // t_token* token = malloc(sizeof(t_token));
    input = formate_line(line);
	if (input && *input)
    	add_history(input);
    tools->cmd = get_tokens(input);
	line_args(tools);
	g_data->syn_err = synatx_err(tools);
    // Print tokens
    // t_token* current_token = tools->cmd;
    // while (current_token != NULL) {
    //     printf("%s [%d]\n", current_token->input, current_token->type);
    //     current_token = current_token->next;
    // }
	t_parser *pars;
	// printf("**********expand**********\n");
	// printf("before: \'\'try\'\' after: \n%s\n", rm_quotes("\'\'try\'\'"));
	// printf("\n**************************\n");
	if (g_data->syn_err == 1)
	{
		pars = fill_parse_struct(tools);
		expand_flag(pars);
		expension(pars, tools);
		// print_parser(pars);
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
	// print_list(g_data->command);
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
			// print_list(g_data->command);
		}
	}
	ft_free_all(NULL, g_data->exit);
}