#include "../include/minishell.h"

int ft_check_env_var(char *env, char *var)
{
    int i;

    i = 0;
    while (env[i] && var[i])
    {
        if (env[i] != var[i])
            break;
        i++;
    }
    if (!var[i] && env[i] == '=')
        return (0);
    return (-1);
}

void ft_lst_del_env_node(t_env *to_del)
{
    t_env *head;
    t_env *next;

    head = data->new_env;
    next = to_del->next;
    while (head->next && head->next != to_del)
        head = head->next;
    free(to_del->value);
    free(to_del);
    head->next = next;
}

int ft_unset(char *var)
{
    t_env *head;
    t_env *tmp;

    if (!var || *var == '\0')
        ft_free_all("unset: : invalid parameter\n", 1);
    head = data->new_env;
    while (head)
    {
        tmp = head->next;
        if (ft_check_env_var(head->value, var) == 0)
            ft_lst_del_env_node(head);
        head = tmp;
    }
    return (0);
}
