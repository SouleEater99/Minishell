// #include "../include/minishell.h"
#include "../libraries/minishell.h"

t_env *ft_lst_create_env_node(char *str)
{
    t_env *new;

    if (!str)
        return (NULL);
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

void    ft_create_minimal_env()
{
    t_env *next;
    char    *pwd;
    char    *tmp;

    data->new_env = ft_lst_create_env_node("SHLVL=1");
    next = ft_lst_create_env_node("PATH=/bin:/usr/local/bin:/usr/bin:/sbin:/bin");
    ft_lst_add_back_env_node(next);
    pwd = getcwd(NULL, 0);
    if (pwd)
    {
        tmp = ft_strjoin("PWD=", pwd);
        next = ft_lst_create_env_node(tmp);
        ft_lst_add_back_env_node(next);
        free(pwd);
        free(tmp);
    }
}

char    *ft_update_shlvl(char *str)
{
    int     num;
    char    *tmp;
    char    *tmp2;

    if (!str)
        return (NULL);
    num = ft_atoi(str);
    num++;
    tmp = ft_itoa(num);
    if (!tmp)
        return (NULL);
    tmp2 = ft_strjoin("SHLVL=", tmp);
    free(tmp);
    return (tmp2);
}

void ft_create_new_env()
{
    int i;
    char    *tmp;
    t_env *next;

    i = 0;
    if (!data->old_env || !data->old_env[0])
    {
        ft_create_minimal_env();
        return ;
    }
    data->new_env = ft_lst_create_env_node(data->old_env[i++]);
    while (data->old_env[i])
    {
        if (ft_check_env_var(data->old_env[i], "SHLVL") == 0)
        {
            tmp = ft_update_shlvl(data->old_env[i] + 6);
            next = ft_lst_create_env_node(tmp);
            free(tmp);
        }
        else
            next = ft_lst_create_env_node(data->old_env[i]);
        ft_lst_add_back_env_node(next);
        i++;
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
