// #include "../include/minishell.h"
#include "../libraries/minishell.h"

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
    
    if (!to_del || !data->new_env)
        return ;
    head = data->new_env;
    next = to_del->next;
    if (head && head == to_del)
        data->new_env = head->next;
    else
        while (head->next && head->next != to_del)
            head = head->next;
    head->next = next;
    free(to_del->value);
    free(to_del);
}

int ft_unset(char *var)
{
    t_env *head;
    t_env *tmp;

    if (!var || *var == '\0')
    {
        return ((data->exit = 0), 0);
    }
    head = data->new_env;
    while (head)
    {
        tmp = head->next;
        if (ft_check_env_var(head->value, var) == 0)
            ft_lst_del_env_node(head);
        head = tmp;
    }
    return ((data->exit = 0), 0);
}
