/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:39:35 by ael-maim          #+#    #+#             */
/*   Updated: 2024/05/14 17:39:38 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ********{****************************************************************** */

#include "../include/minishell.h"

t_mini *data = NULL;

int ft_strcmp(char *s1, char *s2)
{
    if (!s1 || !s2)
        return (-1);
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return (*s1 - *s2);
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

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
    data->new_env = ft_lst_create_env_node(data->old_env[i++]);
    while (data->old_env[i])
    {
        next = ft_lst_create_env_node(data->old_env[i++]);
        ft_lst_add_back_env_node(next);
    }
}

int ft_env()
{
    t_env *env;

    env = data->new_env;
    while (env)
    {
        printf("%s\n", env->value);
        env = env->next;
    }
    return (0);
}

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

    head = data->new_env;
    next = to_del->next;
    while (head->next && head->next != to_del)
        head = head->next;
    free(to_del->value);
    free(to_del);
    head->next = next;
}

int ft_unset(char *var)
{
    t_env *head;
    t_env *tmp;

    if (!var || *var == '\0')
        ft_free_all("unset: : invalid parameter\n", 1);
    head = data->new_env;
    while (head)
    {
        tmp = head->next;
        if (ft_check_env_var(head->value, var) == 0)
            ft_lst_del_env_node(head);
        head = tmp;
    }
    return (0);
}

void ft_lst_free_env()
{
    t_env *head;
    t_env *tmp;

    head = data->new_env;
    while (head)
    {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}

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

t_command *ft_create_cmd_node(char *cmd, char **arg, t_cmd_type type, int red, int append)
{
    t_command *head;

    head = malloc(sizeof(t_command));
    if (!head)
        ft_free_all("error in create node allocation\n", 2);
    head = memset(head, 0, sizeof(t_command));
    head->next = NULL;
    head->command = ft_strdup(cmd);
    if (!head->command)
        ft_free_all("error in create cmd _ strdup allocation\n", 2);
    head->type = type;
    head->redirection = red;
    head->append = append;
    head->in_fd = -1;
    head->out_fd = -1;
    head->args = arg;
    return (head);
}

void ft_cmd_add_to_back(t_command *last)
{
    t_command *head;

    head = data->cmds;
    while (head->next)
        head = head->next;
    head->next = last;
}

void ft_assign_lst_cmd()
{
    t_command *next;
    // char *arg1[] = {"/bin/ls","-l",NULL};
    // char *arg2[] = {NULL};
    // char *arg3[] = {NULL};
    // char *arg4[] = {NULL};

    data->cmds = ft_create_cmd_node("/bin/ls", NULL, SEMICOLON, 0, 0);
    next = ft_create_cmd_node("/bin/pwd", NULL, PIPE, 0, 0);
    ft_cmd_add_to_back(next);
    next = ft_create_cmd_node("/bin/cat", NULL, PIPE, 0, 0);
    ft_cmd_add_to_back(next);
    next = ft_create_cmd_node("/bin/cat", NULL, SEMICOLON, 0, 0);
    ft_cmd_add_to_back(next);
}

void ft_execute_command(t_command *cmd)
{
    cmd->pid = fork();
    if (cmd->pid < 0)
        ft_free_all("error in creating fork ()\n", 2);
    if (cmd->pid == 0)
    {
        if (cmd->in_fd >= 0)
        {
            if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
                ft_free_all("error in-fd\n", 2);
            close(cmd->in_fd);
            cmd->in_fd = -1;
        }
        if (cmd->out_fd >= 0)
        {
            if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
                ft_free_all("error out-fd\n", 2);
            close(cmd->out_fd);
            cmd->out_fd = -1;
        }
        if (execve(cmd->command, cmd->args, NULL) == -1) // here i need to copy new_enp to dobule struct and use it here; some can change the path of the commande;
            ft_free_all("error in execve in normal\n", 2);
    }
}

void    ft_execute_pipe(t_command *cmd)
{
    if (pipe(data->pip) < 0)
        ft_free_all("error in dup2 of pipe\n", 2);
    cmd->pid = fork();
    if (cmd->pid < 0)
        ft_free_all("Error in fork_pip\n", 2);
    if (cmd->pid == 0)
    {
        close(data->pip[0]);
        if (dup2(data->pip[1], STDOUT_FILENO) < 0)
            ft_free_all("error in dup2 of pipe\n", 2);
        close(data->pip[1]);
        if (execve(cmd->command, cmd->args, NULL) == -1) // here i need to copy new_enp to dobule struct and use it here; some can change the path of the commande;
            ft_free_all("error in execve in pip\n", 2);
    }
    close(data->pip[1]);
    if (dup2(data->pip[0], STDIN_FILENO) < 0)
            ft_free_all("error in dup2 of pipe\n", 2);
    close (data->pip[0]);
}

void ft_execute()
{
    t_command *head;

    ft_assign_lst_cmd();
    head = data->cmds;
    printf("++++++++++++++++++++++\n");
    while (head)
    {
        // if (head->type != PIPE)
        //     if (dup2(data->save_stdin, STDIN_FILENO) == - 1)
        //         ft_free_all("Error in dup2 of save_stding \n", 2);
        if (head->type == SEMICOLON || head->type == COMMAND)
            ft_execute_command(head);
        else if (head->type == PIPE)
            ft_execute_pipe(head);
        if (head->type == SEMICOLON || head->type == COMMAND)
            waitpid(head->pid, NULL, 0);
        head = head->next;
    }
    while (wait(NULL) >= 0)
    ;
}

int main(int ac, char **av, char **envp)
{
    (void)av;
    (void)ac;
    (void)envp;
    data = malloc(sizeof(t_mini));
    if (!data)
        exit(1);
    data = ft_memset(data, 0, sizeof(t_mini));
    data->old_env = envp;
    ft_create_new_env();
    data->save_stdin = dup(0);
    data->save_stdout = dup(1);
    if (data->save_stdout == -1 || data->save_stdin == -1)
        ft_free_all("error in dup save_std\n", 2);
    // ft_print_prompt();
    // data->line = readline(NULL);
    // if (!data->line)
    //     ft_free_all("Error in readline\n", 1);
    // while (data->line)
    // {
    //     ft_print_prompt();
    //     free(data->line);
    //     data->line = get_next_line(0);
    //     if (!data->line)
    //         ft_free_all("Error in readline\n", 1);
    // }
    // char *arg[] = {"-n ", "helllo awda", "ali lai", NULL};
    // ft_echo(arg);
    // char *arg_cd[] = {"./../../..../../" ,NULL};
    // ft_cd(arg_cd);
    // ft_pwd();
    // char *arg[] = {"=", "dasd=", NULL};
    // ft_export(arg);
    // ft_env();
    ft_execute();
    //sleep(10000);
    ft_free_all(NULL, 0);
    return (0);
}