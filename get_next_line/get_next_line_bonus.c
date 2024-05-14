/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:45:04 by ael-maim          #+#    #+#             */
/*   Updated: 2023/12/07 19:02:06 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_free(char *to_free)
{
	if (to_free)
		free(to_free);
	return (NULL);
}

char	*get_buffer_line(int fd, char *buckup)
{
	int		rd_byts;
	char	*buffer;

	rd_byts = 1;
	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (ft_free(buckup));
	while (rd_byts > 0 && ft_check_nl(buckup) == 0)
	{
		rd_byts = read(fd, buffer, BUFFER_SIZE);
		if (rd_byts < 0)
			return (free(buffer), ft_free(buckup));
		buffer[rd_byts] = '\0';
		buckup = ft_strjoin(buckup, buffer);
	}
	if (!buckup[0])
		return (free(buffer), ft_free(buckup));
	return (free(buffer), buckup);
}

char	*get_next_line(int fd)
{
	static char	*buckup[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buckup[fd] = get_buffer_line(fd, buckup[fd]);
	if (!buckup[fd])
		return (NULL);
	line = ft_get_nl(buckup[fd]);
	buckup[fd] = ft_get_remaind(buckup[fd]);
	return (line);
}
