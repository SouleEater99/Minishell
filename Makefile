CC = gcc -Wall -Wextra -Werror -lreadline  -fsanitize=address -g3
NAME = minishell
SRC	=	./src/minishell.c \
		./src/minishell_utils.c \
		./src/built_in.c \
		./src/exec_utils.c \
		./src/free.c \
		./src/ft_env.c \
		./src/ft_export.c \
		./src/ft_unset.c \
		./src/parse_utils.c \
		./src/utils.c \

OBJ	=	$(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) 
	$(CC) $(OBJ) -o $(NAME)


clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all
