/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:43:27 by ael-maim          #+#    #+#             */
/*   Updated: 2023/12/07 18:14:41 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_get_remaind(char *buckup);
char	*ft_get_nl(char *buckup);
char	*get_buffer_line(int fd, char *buckup);
char	*ft_strjoin(char *buckup, char *buffer);
int		ft_check_nl(char *buffer);
int		ft_strlen(char *str);
char	*ft_free(char *to_free);

#endif
