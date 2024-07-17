#include "../../include/minishell.h"

int ft_echo(char **arg)
{
    int nl_flag;
    int i;
    int number_of_arg;

    number_of_arg = 0;
    i = 1;
    nl_flag = 0;
    if (!arg || !arg[1] )
        return (printf("\n"),ft_free_all(NULL, 1) , 0);
    while (arg[number_of_arg])
        number_of_arg++;
    if (arg[1] && ft_strcmp("-n", arg[1]) == 0)
        nl_flag = 1;
    if (number_of_arg == 1 && nl_flag == 1)
        return (ft_free_all(NULL, 0) ,0);
    if (nl_flag == 1)
        i = 2;
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

    if (!var)
        return (NULL);
    next = data->new_env;
    while (next && ft_check_env_var(next->value, var) != 0)
        next = next->next;
    if (!next)
        return (NULL);
    i = ft_strlen(var) + 1;
    return (next->value + i);
}

int    ft_change_dir(char *path)
{
    char    *var;
    char    *old_pwd;
    char    *new_pwd;

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return (perror(path), -1);
    if (chdir(path) == -1)
        return (perror(path), -1);
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
        return (perror(path), free(old_pwd), -1);
    var = ft_strjoin("PWD=", new_pwd);
    if (!var)
        return (free(new_pwd), free(old_pwd), -1);
    ft_add_or_update(var);
    free(var);
    var = ft_strjoin("OLDPWD=", old_pwd);
    if (!var)
        return (free(new_pwd), free(old_pwd), -1);
    ft_add_or_update(var);
    free(var);
    return (free(old_pwd), free(new_pwd), 0);
}


int ft_cd(char **arg)
{
    int n_arg;
    char    *path;


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
        if (ft_change_dir(path) == -1)
            return (-1);
    }
    else if (n_arg == 2)
        if (ft_change_dir(arg[1]) == -1)
            return ((data->exit = 1), -1);
    // i need to handle cd - on OLDPWD in env;
    // and cd  which let you go to home; ---> done
    return ((data->exit = 0), 0);
}

