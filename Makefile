CC = cc -Wall -Wextra -Werror -lreadline -fsanitize=address -g3
NAME = minishell
LIBFT = ./lib_ft/libft.a
SRC	=	./src/minishell.c \
			./get_next_line/get_next_line.c \
			./get_next_line/get_next_line_utils.c \

OBJ	=	$(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)

clean :
	rm -rf $(OBJ)
	make -sC ./lib_ft clean

fclean : clean
	rm -rf $(NAME)
	make -sC ./lib_ft fclean

re : fclean all
