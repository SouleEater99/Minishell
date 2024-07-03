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
# include <readline/history.h>
# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define INF 4
# define APPEND 5
# define PIPE 6
# define END 7

# define ERROR 1
# define SUCCESS 0




/*************leak colecter structs***************/

typedef struct AllocatedAddress
{
    void* address;
    struct AllocatedAddress* next;
}   AllocatedAddress;

typedef struct MemoryManager
{
    AllocatedAddress* head;
}   MemoryManager;

void    initMemoryManager(MemoryManager* manager);
void    *ft_malloc(MemoryManager* manager, size_t size);
void    ft_free(MemoryManager   *manager, void    *address);
void    ft_freeAll(MemoryManager    *manager);

/************************************************/

typedef struct	s_token
{
	char			*input;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;


typedef struct s_tools
{
    t_token    *cmd;
    char    **args;
    int     ext;
    
}   t_tools;

typedef struct s_env
{
    char    *value;
    struct s_env *next;
}t_env;

typedef struct s_cmd
{
    char    *cmd;
    char    *path;
    char    *here_doc;
    char    **arg;
    int     infile;
    int     outfile;
    struct s_cmd *next;
}t_cmd;


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
    t_tools *tools;
    t_cmd   *cmd;
} t_mini;

extern t_mini *data;

/****************utils*********************/
int     ft_strcmp(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strdup(const char *str);
void	*ft_memset(void *str, int c, size_t n);

/****************free*********************/
void ft_lst_free_env();
void ft_free_all(char *error, int status);
void initMemoryManager(MemoryManager* manager);
void    *ft_malloc(MemoryManager* manager, size_t size);
void    ft_free(MemoryManager   *manager, void    *address);
void    ft_freeAll(MemoryManager    *manager);



/******************parsing******************/
t_token	*get_tokens(char *line);
void    get_tokens_type(t_token *token, int sep);
int     alloc(char *line, int *start);
t_token	*next_token(char *line, int *i);
void	ft_skip_space(const char *str, int *i);
int		ignore_sep(char *line, int i);
int		quotes(char *line, int index);
int		is_sep(char *line, int i);
char	*allocate_line(char *line);
char	*formate_line(char *line);
t_token	*prev_sep(t_token *token, int skip);
int		is_type(t_token *token, int type);
int		is_last_valid_arg(t_token *token);
int		is_types(t_token *token, char *types);



/******************built_in******************/
int ft_echo(char **arg);
int ft_pwd();
void ft_exit();
int ft_cd(char **arg);
int ft_env();
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

/******************execute******************/
void    ft_print_prompt();



#endif