/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:52 by samsaafi          #+#    #+#             */
/*   Updated: 2024/06/01 09:54:41 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
#define MINI_SHELL_H

# include <fcntl.h>                                             // for open sytem call and others system calls
# include <errno.h>                                     // for errors specification
# include <stdio.h>                                     // for printf
# include <stdlib.h>                                    // for malloc and free
# include <signal.h>                                    // for kill and signal handling
# include <unistd.h>                                    // for system calls
# include <dirent.h>                                    // for directory handling
# include <stdbool.h>                                   // for boolean vars
# include <sys/types.h>                                 // 
# include <sys/wait.h>                                  // for wating child process to terminate execution
# include <readline/history.h>                  // readline GNU library
# include <readline/readline.h>                 // readline GNU library
#define EMPTY 0
#define CMD 1
#define ARG 2
#define TRUNC 3
#define INF 4
#define APPEND 5
#define HEREDOC 6
#define PIPE 7

#define ERROR 1
#define SUCCESS 0

/*************leak colecter structs***************/
typedef struct AllocatedAddress
{
    void *address;
    struct AllocatedAddress *next;
} AllocatedAddress;

typedef struct MemoryManager
{
    AllocatedAddress *head;
} MemoryManager;

void initMemoryManager(MemoryManager *manager);
void *ft_malloc(MemoryManager *manager, size_t size);
void ft_free(MemoryManager *manager, void *address);
void ft_freeAll(MemoryManager *manager);

/************************************************/

typedef struct s_command
{
    int type;
    // char                         *file;
    int in_file;
    int out_file;
    int quoted;
    char *value;
    char **args;
    struct s_command *next;
} t_command;

typedef struct s_env
{
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_data
{
    int i;
    int j;
    int ac;
    t_env *env;
    char **av;
    char *prompt;
    char *line;
    t_command *command;
    char *path;
    pid_t pid;
    char *cwd;
    char **old_env;
    int **pip;
    char **tab;
    int pipe_line[2];
    int i_pip;
    int n_cmd;
    int n_heredoc;
    int save_stdin;
    int save_stdout;
    t_env *new_env;
    char **exec_env;
    int exit;

}t_data;


/************************************************/

typedef struct s_token
{
    char *input;
    int type;
    struct s_token *prev;
    struct s_token *next;
} t_token;

typedef struct s_parser
{
    char *str;
    int type;
    char **args;
    int index;
    struct s_parser *next;
} t_parser;

typedef struct s_tools
{
    t_token *cmd;
    t_parser *parse;
    int ext;

} t_tools;




extern t_data *data; // for global var


/******************lexer******************/
void get_tokens_type(t_token *token, int sep);


// execution


int     ft_strcmp(char *s1, char *s2);
int		ft_check_heredoc(t_command *cmd);
void	ft_close_free_heredoc_pipes();
void 	ft_free_all(char *str, int status);
int	ft_init_heredoc_pip();
void	ft_write_in_pipes(t_command *cmd);
void	ft_print_pip_content();
void	ft_heredoc();
int		ft_numbers_of_cmd(t_command *cmd);
void	ft_setup_dup2(t_command *cmd);
t_command	*ft_return_next_cmd(t_command *cmd);
void	ft_execute_cmd(t_command *cmd);
t_command	*ft_return_cmd_index(t_command *cmd);
void	ft_execution();
void ft_lst_free_env();

// built_in
void ft_lst_free_env();
int ft_echo(char **arg);
void ft_pwd();
void ft_exit();
int ft_cd(char **arg);
void ft_env();
int ft_unset(char *var);
int ft_export(char **arg);
void ft_create_new_env();
void ft_lst_add_back_env_node(t_env *next);
t_env *ft_lst_create_env_node(char *str);
int ft_check_export_arg(char *str);
char *ft_get_identifier(char *var);
void ft_add_or_update(char *var);
int ft_check_env_var(char *env, char *var);
void ft_lst_del_env_node(t_env *to_del);
char    *ft_strjoin_path(char const *s1, char const *s2, char c);
void    ft_free_tab(char **tab);
size_t  ft_strlen_delimiter(const char *str);
void	ft_check_path(char *path);
void		ft_execute_builtin_child(t_command *cmd);
int		ft_check_is_builtin_parent(t_command *cmd);
int		ft_check_is_builtin_child(t_command *cmd);
char *fetch_path_of_cmd(t_env *env, char *cmd);
void	ft_execute_builtin_parent(t_command *cmd);
void    ft_create_minimal_env();
char    **ft_get_env_in_tab();
int     ft_calc_env();
char    *ft_update_shlvl(char *str);
void    ft_print_export();
char    *ft_expand_var(char *var);

/******************execute******************/
void    ft_print_prompt();



#endif
