// #include "../include/minishell.h"
#include "../libraries/minishell.h"

int ft_check_export_arg(char *str)
{
    int i;

    i = 0;
    if (!str[i] || str[i] == '=' || !((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')))
        if (str[i] != '_')
            return (0);
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            return (0);
        if (str[i++] == '=')
            return (1);
    }
    return (-1);
}

char *ft_get_identifier(char *var)
{
    int i;
    char *new;

    i = 0;
    while (var[i] && var[i] != '=')
        i++;
    new = malloc(i + 1);
    if (!new)
        ft_free_all("Error in allocation of identifier of export\n", 1);
    i = 0;
    while (var[i] && var[i] != '=')
    {
        new[i] = var[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

void ft_add_or_update(char *var)
{
    t_env *head;
    char *identifier;

    identifier = ft_get_identifier(var);
    head = data->new_env;
    while (head)
    {
        if (ft_check_env_var(head->value, identifier) == 0)
        {
            free(head->value);
            free(identifier);
            head->value = ft_strdup(var);
            if (!head->value)
                ft_free_all("error in allocation export\n", 1);
            return;
        }
        head = head->next;
    }
    free(identifier);
    if (data->new_env)
        head = ft_lst_create_env_node(var);
    else
        data->new_env = ft_lst_create_env_node(var);
    ft_lst_add_back_env_node(head);
}

int     ft_calc_env()
{
    int     i;
    t_env   *next;

    i = 0;
    next = data->new_env;
    while (next)
    {
        next = next->next;
        i++;
    }
    return (i);
}

char    **ft_get_env_in_tab()
{
    char    **tab;
    int     i;
    t_env   *next;

    i = ft_calc_env();
    tab = malloc(sizeof(char *) * (i + 1));
    next = data->new_env;
    i = 0;
    while (next)
    {
        tab[i++] = next->value;
        next = next->next;
    }
    tab[i] = NULL;
    return (tab);   
}

void    ft_print_export()
{
    char    **tab;
    char    *tmp;
    int     n;

    tab = ft_get_env_in_tab();
    data->i = 0;
    n = ft_calc_env();
    while (data->i < n)
    {
        data->j = 0;
        while (tab[data->j])
        {
            if (tab[data->j + 1] && ft_strcmp(tab[data->j], tab[data->j + 1]) > 0)
            {
                tmp = tab[data->j];
                tab[data->j] = tab[data->j + 1];
                tab[data->j + 1] = tmp;
            }
            data->j++;
        }
        data->i++;
    }
    data->i = 0;
    while (tab[data->i])
        printf("declare -x %s\n", tab[data->i++]);
    free(tab);
}



int ft_export(char **arg)
{
    static unsigned int i;
    int n;

    n = 0;
    if (arg)
        while (arg[n])
            n++;
    if (!arg || !*arg || n == 1)
    {
        if (data->new_env)
            ft_print_export();
        return ((data->exit = 0), 0);
    }
    if (arg[i])
    {
        if (ft_check_export_arg(arg[i]) == 0)
        {
            ft_putstr_fd("export: has not a valid identifier\n", 2);
            data->exit = 1;
        }
        else if (ft_check_export_arg(arg[i]) == 1)
            ft_add_or_update(arg[i]);
        i++;
        ft_export(arg);
    }
    i = 0;
    return ((data->exit = 0), 0);
}

