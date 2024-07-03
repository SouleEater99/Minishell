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
        return (printf("\n"), 0);
    while (arg[number_of_arg])
        number_of_arg++;
    if (ft_strcmp("-n", arg[0]) == 0)
        nl_flag = 1;
    if (number_of_arg == 1 && nl_flag == 1)
        return (0);
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
    return (0);
}

int ft_pwd()
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if (!pwd)
        return (ft_putstr_fd("error in allocation of pwd\n", 2), 1);
    printf("%s\n", pwd);
    free(pwd);
    return (0);
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
    if (n_arg > 1)
        return (ft_putstr_fd("bash: cd: too many arguments\n", 2), 1);
    if (chdir(arg[0]) == -1)
        return (ft_putstr_fd("No such file or directory\n", 2), 1);
    // i need to handle cd - on OLDPWD in env;
    // and cd  which let you go to home;
    return (0);
}

