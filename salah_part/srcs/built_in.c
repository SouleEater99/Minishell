#include "../include/minishell.h"


int ft_echo(char **arg)
{
    int nl_flag;
    int i;
    int number_of_arg;

    number_of_arg = 0;
    i = 0;
    nl_flag = 0;
    if (!arg)
        return (printf("\n"),ft_free_all(NULL, 1) , 0);
    while (arg[number_of_arg])
        number_of_arg++;
    if (arg[1] && ft_strcmp("-n", arg[1]) == 0)
        nl_flag = 1;
    if (number_of_arg == 1 && nl_flag == 1)
        return (ft_free_all(NULL, 0) ,0);
    if (nl_flag == 1)
        i = 1;
    while (arg[i])
    {
        printf("%s", arg[i]);
        if (i++ < number_of_arg - 1)
            printf(" ");
    }
    if (nl_flag == 0)
        printf("\n");
    return (ft_free_all(NULL, 0), 0);
}

void    ft_pwd()
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if (!pwd)
        ft_free_all("error in allocation of pwd\n", 1);
    printf("%s\n", pwd);
    free(pwd);
    ft_free_all (NULL, 0);
}

void ft_exit()
{
    data->exit = 0;
    ft_free_all(NULL, 0);
}

char    *ft_expand_var(char *var)
{
    t_env *next;
    int i;

    next = data->new_env;
    while (next && ft_check_env_var(next->value, var) != 0)
        next = next->next;
    if (!next)
        return (NULL);
    i = ft_strlen(var) + 1;
    return (next->value + i);
}

int ft_cd(char **arg)
{
    int n_arg;
    char    *path;
    char    **tab;


    n_arg = 0;
    while (arg[n_arg])
        n_arg++;
    if (n_arg > 2)
        return (ft_putstr_fd("bash: cd: too many arguments\n", 2), (data->exit = 1), 0);
    else if (n_arg == 1)
    {
        path = ft_expand_var("HOME");
        if (!path)
            return ((data->exit = 0),ft_putstr_fd("bash: cd: HOME not set", 2), 0);
        if (chdir(path) == -1)
            return (ft_putstr_fd("No such file or directory\n", 2), (data->exit = 1), 0);
        path = ft_strjoin("PWD=", path);
        tab = ft_split(path, ' ');
        ft_export(tab);
        free(path);
        free(tab[0]);
        free(tab);
    }
    else if (n_arg == 2)
    {
        if (chdir(arg[1]) == -1)
            return (ft_putstr_fd("No such file or directory\n", 2), (data->exit = 1), 0);
        path = ft_strjoin("PWD=", arg[1]);
        tab = ft_split(path, ' ');
        printf("++++++++++++|tab[0] = %s|+++++++++|\n", tab[0]);
        ft_export(tab);
        free(path);
        free(tab[0]);
        free(tab);
    }
    // i need to handle cd - on OLDPWD in env;
    // and cd  which let you go to home; ---> done
    return ((data->exit = 0), 0);
}

