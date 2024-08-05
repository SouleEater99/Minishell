/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:52 by samsaafi          #+#    #+#             */
/*   Updated: 2024/07/30 11:00:02 by samsaafi         ###   ########.fr       */
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
# include <sys/wait.h>                              // for wating child process to terminate execution
#include <sys/stat.h>
# include <readline/history.h>                  // readline GNU library
# include <readline/readline.h>                 // readline GNU library
# include "./lib_ft/libft.h"
# include "./get_next_line.h"

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"

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
    char *value;
    char **args;
    struct s_command *next;
} t_command;

typedef struct s_env
{
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_token
{
    char *input;
    int type;
    struct s_token *prev;
    struct s_token *next;
} t_token;

typedef struct s_data
{
    int i;
    int j;
    int ac;
    // t_env *env;
    char **av;
    char **old_env;
    char *prompt;
    char *line;
    t_command *command;
    t_token *token;
    char *path;
    pid_t pid;
    char    *hostname;
    char    *username;
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
    int syn_err;
    int exit;

}t_data;


/************************************************/

typedef struct s_var
{
	int	sq;
	int	dq;
	char	*newstr;
	char *var;
}	t_var;

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
    t_env   *env;
    int ext;

} t_tools;

char	*expand_str(char *str,t_env *envp);


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
int ft_exit(char **args);
int ft_cd(char **arg);
void ft_env();
int ft_unset_from_env(char *var);
int ft_unset(char **args);
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
void	ft_init_minishell(int ac, char **av, char **env);
char    *ft_prompt();
int     ft_init_username();
char    *ft_get_uid();
void    ft_init_hostname();
int    ft_change_dir(char *path);
void	ft_free_command(t_command *cmd);
void	ft_free_utils();
char **ft_copy_tab(char **arg, t_parser *pars, t_token *token);
void	ft_sig_handler(int sig);
void	ft_sig_handler_child(int sig);
void	ft_print_to_nl(char *str);
int	is_a_directory(char *path);







/****************** parsing *****************/
char	*expand_str(char *str, t_env *envp);
char    *apend_char_str(char *str, char c);
char	*ft_appand(char *var, char *newstr,t_env *envp);
char	*add_to_str(char *str, t_var var, int *i);
char	*rm_quotes(char *str);
void expension(t_parser *parser, t_tools *tools);


void exit_fork(int status);
/******************execute******************/
void    ft_print_prompt();



#endif
