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
/* ************************************************************************** */

#include "./minishell.h"

int main(int ac, char **av, char **envp)
{
    char *line;

    line = readline("HERE THE PROMPOT : ");
    while (line)
    {
        printf("line = %s\n", line);
        free(line);
        line = readline("HERE THE PROMPOT : ");
    }
    free(line);
}
