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

t_mini *data;

int ft_strlen_protected(char *str)
{
    int i;

    i = 0;
    if (!str)
        return (i);
    while (str[i])
        i++;
    return (i);
}

char *ft_strjoin_paranthesis(char *s1, char *s2)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    new = malloc(ft_strlen_protected(s1) + ft_strlen_protected(s2) + 2);
    if (!new)
        return (free(s1), free(s2), NULL);
    while (s1 && s1[i] && s1[i])
    {
        new[i] = s1[i];
        i++;
    }
    if (s1[i] == '\n')
    {
        new[i++] = ';';
        new[i++] = '\n';
    }
    while (s2 && s2[j])
        new[i++] = s2[j++];
    new[i] = '\0';
    return (free(s1), free(s2), new);
}

void ft_free_node(t_node *root)
{
    free(root);
}

void ft_free_all(char *error, int status)
{
    if (data)
    {
        if (data->line)
            free(data->line);
        if (data->result)
            free(data->result);
        if (data->next_line)
            free(data->next_line);
        if (data->root)
            ft_free_node(data->root);
    }
    ft_putstr_fd(error, 2);
    exit(status);
}

void ft_handle_qoutes_end(char qoute)
{
    char *backup;
    if (data->line[data->i] == qoute && data->qoutes_flag == 0)
    {
        data->qoutes_flag++;
        data->i++;
    }
    while (data->line[data->i])
    {
        if (data->line[data->i] == qoute && data->qoutes_flag > 0)
        {
            data->qoutes_flag--;
            return;
        }
        data->i++;
    }
    data->next_line = get_next_line(0);
    if (!data->next_line)
        ft_free_all("Error in readline of qoutes\n", 1);
    backup = ft_strjoin(data->line, data->next_line);
    if (!backup)
        ft_free_all("Error in strjoin on qoutes\n", 1);
    free(data->line);
    free(data->next_line);
    data->next_line = NULL;
    data->line = backup;
    ft_handle_qoutes_end(qoute);
}

void ft_handle_parenthesis(char parenthesis)
{
    char *backup;

    while (data->line[data->i])
    {
        if (data->line[data->i] == parenthesis)
            data->parenthesis_flag++;
        if (data->line[data->i] == '\'' || data->line[data->i] == '\"')
        {
            ft_handle_qoutes_end(data->line[data->i]);
        }
        else if (data->line[data->i] == ')' && data->parenthesis_flag == 1)
        {
            data->parenthesis_flag--;
            return;
        }
        else if (data->line[data->i] == ')')
            data->parenthesis_flag--;
        data->i++;
    }
    if (data->parenthesis_flag < 0)
        ft_free_all("bash: syntax error near unexpected token ')' \n", 2);
    else if (data->parenthesis_flag > 0)
    {
        data->next_line = get_next_line(0);
        if (!data->next_line)
            ft_free_all("error in allocation of data->next_line\n", 1);
        backup = ft_strjoin(data->line, data->next_line);
        if (!backup)
            ft_free_all("error in strjoin backup\n", 1);
        free(data->line);
        free(data->next_line);
        data->next_line = NULL;
        data->line = backup;
        ft_handle_parenthesis(parenthesis);
    }
}

void ft_check_line()
{

    data->i = -1;
    while (data->line[++data->i])
    {
        if (data->line[data->i] == '\'' || data->line[data->i] == '\"')
            ft_handle_qoutes_end(data->line[data->i]);
        if (data->line[data->i] == '(')
            ft_handle_parenthesis(data->line[data->i]);
    }
}

int main()
{
    data = malloc(sizeof(t_mini));
    if (!data)
        exit(1);
    data = ft_memset(data, 0, sizeof(t_mini));
    ft_putstr_fd("➜ Minishell git:(main): ", 1);
    data->line = get_next_line(0);
    if (!data->line)
        ft_free_all("Error in readline\n", 1);
    while (data->line)
    {
        ft_check_line();
        printf("data.line = %s\n", data->line);

        free(data->line);
        data->line = get_next_line(0);
        if (!data->line)
            ft_free_all("Error in readline\n", 1);
    }
}
