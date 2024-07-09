// #include "../include/minishell.h"
#include "../libraries/minishell.h"


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
    ft_free_all(NULL, 0);
}

int ft_cd(char **arg)
{
    int n_arg;

    n_arg = 0;
    while (arg[n_arg])
        n_arg++;
    if (n_arg > 2)
        return (ft_free_all("bash: cd: too many arguments\n", 1), 1);
    if (chdir(arg[1]) == -1)
        return (ft_free_all("No such file or directory\n", 1), 1);
    // i need to handle cd - on OLDPWD in env;
    // and cd  which let you go to home;
    return (ft_free_all(NULL, 0), 0);
}

