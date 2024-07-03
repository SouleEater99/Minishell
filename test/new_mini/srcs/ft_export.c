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
    head = ft_lst_create_env_node(var);
    ft_lst_add_back_env_node(head);
}

int ft_export(char **arg)
{
    static unsigned int i;
    int n;

    n = 0;
    if (!arg)
    {
        ft_env();
        return (0);
    }
    while (arg[n])
        n++;
    if (arg[i])
    {
        if (ft_check_export_arg(arg[i]) == 0)
            ft_putstr_fd("export: has not a valid identifier\n", 2);
        else if (ft_check_export_arg(arg[i]) == 1)
            ft_add_or_update(arg[i]);
        i++;
        ft_export(arg);
    }
    i = 0;
    return (0);
}