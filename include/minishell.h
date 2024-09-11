/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:32:52 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/08 22:32:48 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./get_next_line.h"
# include "./lib_ft/libft.h"
# include <dirent.h>            // for directory handling
# include <errno.h>             // for errors specification
# include <fcntl.h>             // for open sytem call and others system calls
# include <readline/history.h>  // readline GNU library
# include <readline/readline.h> // readline GNU library
# include <signal.h>            // for kill and signal handling
# include <stdbool.h>           // for boolean vars
# include <stdio.h>             // for printf
# include <stdlib.h>            // for malloc and free
# include <sys/stat.h>
# include <sys/types.h> //
# include <sys/wait.h>  // for wating child process to terminate execution
# include <unistd.h>    // for system calls

# define RESET "\x1B[0m"
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define MAGENTA "\x1B[35m"
# define CYAN "\x1B[36m"

# define FILENAME 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define INF 4
# define APPEND 5
# define HEREDOC 6
# define PIPE 7

# define ERROR 1
# define SUCCESS 0

typedef struct s_command
{
	int					type;
	char				*value;
	char				**args;
	int					index;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	char				*input;
	int					type;
	char				**args;
	struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef struct s_data
{
	int					i;
	int					j;
	int					ac;
	// t_env *env;
	char				**av;
	char				**old_env;
	char				*prompt;
	char				*line;
	t_command			*command;
	t_token				*token;
	char				*path;
	pid_t				pid;
	int					**pip;
	char				**tab;
	int					pipe_line[2];
	int					i_pip;
	int					n_cmd;
	int					n_heredoc;
	int					save_stdin;
	int					save_stdout;
	t_env				*new_env;
	char				**exec_env;
	int					syn_err;
	int					exit;

}						t_data;

/************************************************/

typedef struct s_var
{
	int					sq;
	int					dq;
	char				*newstr;
	char				*var;
}						t_var;

typedef struct s_vars
{
    int        n;
    int        i;
    int        dblqt;
    int        j;
    int        sinqt;
}						t_vars;

typedef struct s_parser
{
	char				*str;
	int					type;
	char				**args;
	int					index;
	struct s_parser		*next;
}						t_parser;

typedef struct s_tools
{
	t_token				*cmd;
	t_env				*env;
	int					ext;

}						t_tools;


extern t_data	*g_data;

/******************parsing******************/
void					exit_fork(int status);

void					*ft_memdel(void *ptr);
void					free_token_list(t_token *token);
void					free_parser_list(t_parser *pars);
int						alloc(char *line, int *start);
char					*allocate_line(char *line);
char					*formate_line(char *line);
char					**line_tab(t_token *start);
t_parser				*fill_parse_struct(t_tools *tools);
void					ft_putendl_fd(char *s, int fd);
int						synatx_err(t_tools *tools);
void					ft_skip_space(const char *str, int *i);
int						ignore_sep(char *line, int i);
t_token					*prev_sep(t_token *token, int skip);
int						is_type(t_token *token, int type);
int						is_last_valid_arg(t_token *token);
int						is_sep(char *line, int i);
int						is_types(t_token *token, char *types);
void					line_args(t_tools *mini);
void					get_tokens_type(t_token *token, int sep);
t_token					*next_token(char *line, int *i);
t_token					*get_tokens(char *line);
void					split_token_input(t_token *token);

/***********************expension***************/
char					*apend_char_str(char *str, char c);
char					*ft_appand(char *var, char *newstr);
char					*add_to_str(char *str, t_var var, int *i);
char					*expand_str(char *str,int flag);
void					ignore_empty_cmd_arg(t_token **token);
void					expension(t_token *token);
int						quotes(char *line, int index);
char					*rm_quotes(char *str);
void					expand_flag(t_parser *parser);
void					del_q(t_parser *parse);
char					**ft_split_it(char *str);

// execution

int						ft_strcmp(char *s1, char *s2);
int						ft_check_heredoc(t_command *cmd);
void					ft_close_free_heredoc_pipes(void);
void					ft_free_all(char *str, int status);
int						ft_init_heredoc_pip(void);
void					ft_write_in_pipes(t_command *cmd);
void					ft_print_pip_content(void);
void					ft_heredoc(void);
int						ft_numbers_of_cmd(t_command *cmd);
void					ft_setup_dup2(t_command *cmd);
t_command				*ft_return_next_cmd(t_command *cmd);
int						ft_execute_cmd(t_command *cmd);
t_command				*ft_return_cmd_index(t_command *cmd);
void					ft_execution(void);
void					ft_lst_free_env(void);

// built_in
void					ft_lst_free_env(void);
int						ft_echo(char **arg);
void					ft_pwd(void);
int						ft_exit(char **args);
int						ft_cd(char **arg);
void					ft_env(void);
int						ft_unset_from_env(char *var);
int						ft_unset(char **args);
int						ft_export(char **arg);
void					ft_create_new_env(void);
void					ft_lst_add_back_env_node(t_env *next);
t_env					*ft_lst_create_env_node(char *str);
int						ft_check_export_arg(char *str);
char					*ft_get_identifier(char *var);
int						ft_add_or_update(char *var);
int						ft_check_env_var(char *env, char *var);
void					ft_lst_del_env_node(t_env *to_del);
char					*ft_strjoin_path(char const *s1, char const *s2,
							char c);
void					ft_free_tab(char **tab);
size_t					ft_strlen_delimiter(const char *str);
void					ft_check_path(char *path);
void					ft_execute_builtin_child(t_command *cmd);
int						ft_check_is_builtin_parent(t_command *cmd);
int						ft_check_is_builtin_child(t_command *cmd);
char					*fetch_path_of_cmd(t_env *env, char *cmd);
void					ft_execute_builtin_parent(t_command *cmd);
void					ft_create_minimal_env(void);
char					**ft_get_env_in_tab(void);
int						ft_calc_env(void);
char					*ft_update_shlvl(char *str);
void					ft_print_export(void);
char					*ft_expand_var(char *var);
void					ft_init_minishell(int ac, char **av, char **env);
char					*ft_prompt(int flag);
int						ft_init_username(void);
char					*ft_get_uid(void);
void					ft_init_hostname(void);
int						ft_change_dir(char *path);
void					ft_free_command(t_command *cmd);
void					ft_free_utils(void);
char					**ft_copy_tab(char **arg, t_parser *pars,
							t_token *token);
void					ft_sig_handler(int sig);
void					ft_sig_handler_child(int sig);
void					ft_print_to_nl(char *str);
int						is_a_directory(char *path);
void					ft_free_pwd(char *new_path, char *old_path);

void					ft_add_back_cmd(t_command *command);
t_command				*ft_copy_pars_to_cmd(t_parser *pars);
void					ft_replace_our_struct(t_parser *pars);
void					print_banner(void);
void					ft_init_minishell(int ac, char **av, char **env);
void					exit_fork(int status);
int						check_cmd_is_path(char *cmd);
void					ft_free_utils_2(void);
int						ft_tab_lenght(char **tab);
void					ft_inti_execution(void);
void					ft_setup_execution(t_command *cmd,
							t_command *cmd_index);
void					setup_dup2_readout(int fd, t_command *cmd);
void					setup_dup2_readin(int fd, t_command *cmd);
void					ft_check_exit_arg(char **args);
int						ft_is_switch(char *str, char *switches);
void					ft_print_qoutes(char *str);
char					*ft_replace_and_join(char *value,char *var);
char					*ft_update_identifier(char *identifier, char *var);
int						ft_check_equal_env(char *value);
void					get_pwd_from_env();
void					ft_get_next_pip(t_command *cmd);
char					*ft_get_env_last_cmd(t_command *last);
void					ft_update_env_last_cmd();

/****************** -----{ print_list }------ *****************/

void					print_type(int type);
void					print_args(char **args);
void					print_list(t_command *table);

/******************execute******************/
void					ft_print_prompt(void);

void	get_file_types(t_token * token);
void	check_cmd(t_token *token);
int	count_args(t_token *start);

#endif
