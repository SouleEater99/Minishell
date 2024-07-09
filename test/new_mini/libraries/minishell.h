/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:43:39 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/02 11:43:36 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include "./colors.h"
# include "./libft/libft.h" 			// libft functions
# include <fcntl.h>						// for open sytem call and others system calls
# include <errno.h> 					// for errors specification
# include <stdio.h> 					// for printf
# include <stdlib.h> 					// for malloc and free
# include <signal.h> 					// for kill and signal handling
# include <unistd.h> 					// for system calls
# include <dirent.h> 					// for directory handling
# include <stdbool.h> 					// for boolean vars
# include <sys/types.h> 				// 
# include <sys/wait.h> 					// for wating child process to terminate execution
# include <readline/history.h> 			// readline GNU library
# include <readline/readline.h> 		// readline GNU library

typedef enum s_types
{
	CMD,
	RED_OUT,
	RED_IN,
	PIPE,
	LIST,
	BACK,
	ARG,
	OR_OP,
	AND_OP,
	FLE,
	APP,
	HER_DOC
} t_types;

# define TOKEN			0 //CMD 			0
	
# define EXEC 			1
# define RED_OUT 		2
# define RED_IN 		3
# define PIPE 			4
# define LIST 			5
# define BACK 			6
# define ARG 			7
# define OR_OP 			8
# define AND_OP 		9
# define FLE			10
# define APP			11
# define HER_DOC		12

typedef struct s_env
{
	char 				*value;
	struct s_env 		*next;
} t_env;

typedef struct s_command
{
	int					type;
	// char				*file;
	int					in_file;
	int					out_file;
	int					quoted;
	char				*value;
	char 				**args;
	struct s_command	*next;
}				t_command;

typedef struct s_data
{
	int 				i;
	int 				j;
	int					ac;
	t_env				*env;
	char				**av;
	char				*prompt;
	char 				*new_command;
	char  				*special_chars;
	char				**envirenment;
	bool				syntax_error;		// boolean variable for syntax_error
	t_command			*command;
	char				*path;
	pid_t				pid;
	char   				*cwd;
    char    			**old_env;
	int					**pip;
	char				**tab;
    int     			pipe_line[2];
	int					i_pip;
	int					n_cmd;
	int					n_heredoc;
    int     			save_stdin;
    int     			save_stdout;
    t_env   			*new_env;
	char				**exec_env;
	int					exit;

}				t_data;

typedef struct s_token 
{
	int 				i;						// just a normal index for reading the input character by character
	int					prev_type;				// this variable for tracking the type of the previous node 
	int					index;					// to point to the location of the next token
	int 				type;					// for the type of the token PIPE REDER ...
	char 				*value;					// token value if it's a pipe the value is "|"
} t_token;

extern t_data *data;					// for use this global var from all files


// # include <termios.h>
// # include <sys/stat.h>

void			print_prompt(void);
void			free_array(char **array);

// buit-in commands:

int				cd(char *path);
char			*get_prompt(void);
void			pwd(void);
int				env(t_env *env);
int				echo(char **cmd);
int				export(char *cmd);
// int				built_in_cmd(char **parsedcmd);


// general purpose utiles

char 			*skip_white_spaces(char *command);
char 			*skip_command(char *command);


// utiles for linked list: 

void			add_back_list(t_command **lst, t_command *new);
t_command		*new_node(int type, char *value);
void			clear_list(t_command **lst);

// parsing and toknizing functions

int				parse_command(char *command);
t_command 		*tokenize_command(char *commads);
char 			*get_token_value(t_token *token, char *commads);
// int 			get_token_type(t_token *token);
void			parentheses(char *command);
int 			check_syntax(char *command);


// parsing utiles

int 			ft_strisalnum(char *str);

int				exec_command(t_command *commands_list);
char	*ft_strnstr_l(const char *big, const char *little, size_t len);
char *get_env_element(char *env_var);



// execution


int ft_strcmp(char *s1, char *s2);
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

/******************execute******************/
void    ft_print_prompt();




#endif
