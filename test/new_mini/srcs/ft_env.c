// #include "../include/minishell.h"
#include "../libraries/minishell.h"

t_env *ft_lst_create_env_node(char *str)
{
    t_env *new;

    new = malloc(sizeof(t_env));
    if (!new)
        ft_free_all("Error in allocation of new_node in new_env\n", 1);
    new->value = ft_strdup(str);
    if (!new->value)
    {
        free(new);
        ft_free_all("Error in allocation in new_value\n", 1);
    }
    new->next = NULL;
    return (new);
}

void ft_lst_add_back_env_node(t_env *next)
{
    t_env *head;

    if (next)
    {
        head = data->new_env;
        while (head->next)
            head = head->next;
        head->next = next;
    }
}

void ft_create_new_env()
{
    int i;
    t_env *next;

    i = 0;
    if (!data->old_env || !data->old_env[0])
    {
        data->env = NULL;
        return ;
    }
    data->new_env = ft_lst_create_env_node(data->old_env[i++]);
    while (data->old_env[i])
    {
        next = ft_lst_create_env_node(data->old_env[i++]);
        ft_lst_add_back_env_node(next);
    }
}

void ft_env()
{
    t_env *env;

    env = data->new_env;
    while (env)
    {
        printf("%s\n", env->value);
        env = env->next;
    }
    ft_free_all(NULL, 0);
}
