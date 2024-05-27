#ifndef MINISHELL_H
#define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <readline/readline.h>
# include "../get_next_line/get_next_line.h"
# include    "../lib_ft/libft.h"


typedef enum s_cmd_type
{
    OR,
    AND,
    PIPE,
    COMMAND,
    SEMICOLON,
    BACKGROUND,
    PARENTHESIS,
    CURLY_BRACKET,
}t_cmd_type;

typedef struct s_command
{
    char                *command;
    char                *path;
    char                **args;
    t_cmd_type          type;
    int                 redirection;
    int                 append;
    int                 in_fd;
    int                 out_fd;
    pid_t               pid;
    char                *here_doc;
    char                *limiter;
    struct s_command    *next; 
}t_command;

typedef struct s_env
{
    char    *value;
    struct s_env *next;
}t_env;

typedef struct s_mini
{
    char    *line;
    char    *next_line;
    char    *result;
    char    *cwd;
    char    **old_env;
    int     i;
    int     pip[2];
    int     save_stdin;
    int     save_stdout;
    int     parenthesis_flag;
    int     qoutes_flag;
    t_env   *new_env;
    t_command    *cmds;
} t_mini;

extern t_mini *data;

void ft_free_lst_cmd(t_command *cmds);
void ft_free_all(char *error, int status);
void ft_handle_qoutes_end(char qoute);
void ft_handle_parenthesis(char parenthesis);
void ft_check_line();
void    ft_print_prompt();
void    ft_lst_free_env();



#endif