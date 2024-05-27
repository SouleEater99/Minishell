/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:43:27 by ael-maim          #+#    #+#             */
/*   Updated: 2024/03/26 16:46:43 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

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
char	*ft_strjoin_nl(char *buckup, char *buffer);
int		ft_check_nl(char *buffer);
int		ft_strlen_nl(char *str);
char	*ft_free_nl(char *to_free);

#endif
