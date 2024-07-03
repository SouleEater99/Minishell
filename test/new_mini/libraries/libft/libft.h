/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumi <yumi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:16:43 by aelkheta          #+#    #+#             */
/*   Updated: 2024/05/27 09:00:26 by yumi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <limits.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1

# endif

typedef struct s_list
{
	int				content;
	struct s_list	*next;
}					t_list;

char				*ft_free(char *buffer);
char				*ft_strjoin(char *s1, char *s2);
char				*ft_strchr(const char *str, int c);
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				*get_next_line(int fd);
char				*get_the_rest(char *buffer);
char				*check_next_line(char *buffer);
char				*reach_the_line(int fd, char *buffer);

long int			ft_atoi(const char *str);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t n, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
void				*ft_memchr(const void *str, int c, size_t n);
int					ft_memcmp(const void *str1, const void *str2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *str1, const void *str2, size_t n);
char				*ft_itoa(int n);
void				*ft_memset(void *str, int c, size_t n);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
char				*ft_strrchr(const char *str, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);
void				*ft_calloc(size_t n, size_t size);
char				*ft_strdup(const char *s);

char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

int					ft_lstsize(t_list *lst);
t_list				*ft_lstnew(int content);
t_list				*ft_lstmap(t_list *lst, int (*f)(int), void (*del)(int));
t_list				*ft_lstlast(t_list *lst);
void				ft_lstiter(t_list *lst, void (*f)(int));
void				ft_lstdelone(t_list *lst, void (*del)(int));
void				ft_lstclear(t_list **lst, void (*del)(int));
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstadd_front(t_list **lst, t_list *new);

#endif