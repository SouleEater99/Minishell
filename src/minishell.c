/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:39:35 by ael-maim          #+#    #+#             */
/*   Updated: 2024/05/14 17:39:38 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ********{****************************************************************** */

#include "../include/minishell.h"

t_mini *data = NULL;

int main(int ac, char **av, char **envp)
{
    (void)av;
    (void)ac;
    (void)envp;
    data = malloc(sizeof(t_mini));
    if (!data)
        exit(1);
    data = ft_memset(data, 0, sizeof(t_mini));
    data->old_env = envp;
    ft_create_new_env();
    data->save_stdin = dup(0);
    data->save_stdout = dup(1);
    if (data->save_stdout == -1 || data->save_stdin == -1)
        ft_free_all("error in dup save_std\n", 2);
    // ft_print_prompt();
    // data->line = readline(NULL);
    // if (!data->line)
    //     ft_free_all("Error in readline\n", 1);
    // while (data->line)
    // {
    //     ft_print_prompt();
    //     free(data->line);
    //     data->line = get_next_line(0);
    //     if (!data->line)
    //         ft_free_all("Error in readline\n", 1);
    // }
    // char *arg[] = {"-n ", "helllo awda", "ali lai", NULL};
    // ft_echo(arg);
    // char *arg_cd[] = {"./../../..../../" ,NULL};
    // ft_cd(arg_cd);
    // ft_pwd();
    // char *arg[] = {"=", "dasd=", NULL};
    // ft_export(arg);
    // ft_env();
    ft_execute();
    //sleep(10000);
    ft_free_all(NULL, 0);
    return (0);
}