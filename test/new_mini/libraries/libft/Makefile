# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 10:22:55 by aelkheta          #+#    #+#              #
#    Updated: 2023/11/22 20:03:04 by aelkheta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

SRCS_M = ft_atoi.c \
		ft_itoa.c \
		ft_putendl_fd.c \
		ft_strlcat.c \
		ft_substr.c \
		ft_bzero.c \
		ft_putnbr_fd.c \
		ft_strlcpy.c \
		ft_tolower.c \
		ft_calloc.c \
		ft_memchr.c \
		ft_putstr_fd.c \
		ft_strlen.c \
		ft_toupper.c \
		ft_isalnum.c \
		ft_memcmp.c \
		ft_split.c \
		ft_strmapi.c \
		ft_isalpha.c \
		ft_memcpy.c \
		ft_strchr.c \
		ft_strncmp.c \
		ft_isascii.c \
		ft_memmove.c \
		ft_strdup.c \
		ft_strnstr.c \
		ft_isdigit.c \
		ft_memset.c \
		ft_striteri.c \
		ft_strrchr.c \
		ft_isprint.c \
		ft_putchar_fd.c \
		ft_strjoin.c \
		ft_strtrim.c \
		get_next_line.c \
		ft_free.c \

SRCS_B = ft_lstmap_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstnew_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstiter_bonus.c \

OBJS_M = $(SRCS_M:.c=.o)
OBJS_B = $(SRCS_B:.c=.o)

$(NAME): $(OBJS_M)
	$(AR) $(NAME) $(OBJS_M)

all: $(NAME)

bonus: $(OBJS_B)

$(OBJS_B): $(SRCS_B)
	$(CC) $(CFLAGS) -c $(SRCS_B)
	$(AR) $(NAME) $(OBJS_B)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS_M) $(OBJS_B)

fclean: clean
	@rm -f $(NAME)

re: fclean all bonus

.PHONY: all bonus clean fclean