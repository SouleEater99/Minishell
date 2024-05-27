CC = cc -Wall -Wextra -Werror 
NAME = pipex
BONUS = pipex_bonus
RM = rm -rf
LIBFT = ./lib_ft/libft.a

SRC = ./src_pipex/pipex.c					\
	  ./src_pipex/pipex_assing.c			\
	  ./src_pipex/pipex_execute.c			\
	  ./src_pipex/pipex_exit.c				\
	  ./src_pipex/pipex_utils.c				\

SRC_B = ./src_pipex/pipex_bonus.c			\
	  ./src_pipex/pipex_assing.c			\
	  ./src_pipex/pipex_execute.c			\
	  ./src_pipex/pipex_exit.c				\
	  ./src_pipex/pipex_utils.c				\
	  ./get_next_line/get_next_line.c		\
	  ./get_next_line/get_next_line_utils.c	\

OBJ = $(SRC:.c=.o)
OBJ_B = $(SRC_B:.c=.o)

all: $(NAME)
	@echo "Build Successful!"


$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)

bonus : $(BONUS)
	@echo "Build Successful bonus!"

$(BONUS) : $(OBJ_B) $(LIBFT)
	$(CC) $(OBJ_B) $(LIBFT) -o $(BONUS)

$(LIBFT) :
	@make -sC ./lib_ft all
	@make -sC ./lib_ft clean
	@echo "Creating libft.a"

clean :
	@$(RM) $(OBJ) $(OBJ_B)
	@echo "Cleanup Complete!"

fclean : clean
	@$(RM) $(NAME) $(BONUS)
	@make -sC ./lib_ft fclean
	@echo "Full Cleanup Complete!"

re: fclean all
	@echo "RE Creating ..."
	@echo "Complete!"


