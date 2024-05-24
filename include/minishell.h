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


typedef enum s_node_type
{
    PIPE,
    COMMAND,
    SEMICOLON,
    PARENTHESIS,
    CURLY_BRACKET,
    AND,
    OR,
    BACKGROUND,

}t_node_type;

typedef struct s_node
{
    t_node_type type;
    char    *value;
    struct s_node  *root;
    struct s_node  *left;
    struct s_node  *right;
}t_node;


typedef struct s_mini
{
    char    *line;
    char    *next_line;
    char    *result;
    char    *cwd;
    int     i;
    int     parenthesis_flag;
    int     qoutes_flag;
    t_node  *root;
    
} t_mini;



#endif