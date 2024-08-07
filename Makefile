NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 #-fsanitize=address
READLINE = -lreadline
LIBFT = ./include/lib_ft/libft.a
SRC =	./srcs/minishell.c \
		./srcs/execution_part/ft_built_in.c \
		./srcs/execution_part/ft_execution.c \
		./srcs/execution_part/ft_env.c \
		./srcs/execution_part/ft_export.c \
		./srcs/execution_part/ft_prompt.c \
		./srcs/execution_part/ft_unset.c \
		./srcs/execution_part/ft_init_minishell.c \
		./srcs/execution_part/ft_signal.c \
		./srcs/execution_part/ft_print_list.c \
		./srcs/execution_part/ft_replace_struct.c \
		./srcs/execution_part/ft_check_cmd_access.c \
		./srcs/execution_part/ft_execute_child.c \
		./srcs/execution_part/ft_execute_utils.c \
		./srcs/execution_part/ft_free.c \
		./srcs/execution_part/ft_heredoc.c \
		./srcs/execution_part/ft_str_utils.c \
		./srcs/utils/get_next_line.c \
		./srcs/utils/get_next_line_utils.c \
		./srcs/utils/utils.c \
		./srcs/expansion.c \
	

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(READLINE) $(CFLAGS) $(LIBFT) -o $(NAME)

$(LIBFT) :
	make -sC ./include/lib_ft all

clean :
	rm -rf $(OBJ)
	make -sC include/lib_ft clean
fclean : clean
	rm -rf $(NAME)
	make -sC include/lib_ft fclean
re : fclean all
