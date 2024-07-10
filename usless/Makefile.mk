# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 09:41:57 by aelkheta          #+#    #+#              #
#    Updated: 2024/06/02 17:01:36 by aelkheta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
RLFLAG = -lreadline
SRCDIR = ./srcs
OBJDIR = ./objs

SRC = 	$(SRCDIR)/minishell.c \
		$(SRCDIR)/built_in_cmd1.c \
		$(SRCDIR)/built_in_cmd2.c \
		$(SRCDIR)/cleanup.c \
		$(SRCDIR)/mini_utiles.c \
		$(SRCDIR)/tokenizer.c \

OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC)) # for pathern substitution // $(patsubst pattern,replacement,text)

LIBFT = ./libraries/libft/libft.a

all: $(NAME)
	@./compiler/compiling
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "Building..."
	@$(CC) $(CFLAGS) $(OBJ) $(RLFLAG) $(LIBFT) -o $@
	@echo "Building done"

clean:
	@echo "Removing object files..."
	@rm -rf $(OBJDIR)
	@echo "Removing object files done"

fclean: clean
	@./compiler/cleaning
	@echo "Removing program name..."
	@rm -f $(NAME)
	@echo "Removing program name done"

re: fclean all

.PHONY: all clean fclean re
