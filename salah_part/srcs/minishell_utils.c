// #include "../include/minishell.h"
#include "../libraries/minishell.h"

void    ft_print_prompt()
{
    data->cwd = getcwd(NULL, 0);
    if (!data->cwd)
        ft_free_all("error in getcwd \n", 1);
    ft_putstr_fd("┌──(username㉿host)-[", 1);
    ft_putstr_fd(data->cwd, 1);
    ft_putstr_fd("]\n└─$ ",1);
    free(data->cwd);
    data->cwd = NULL;
}