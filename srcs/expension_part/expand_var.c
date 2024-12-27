/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:32:11 by samsaafi          #+#    #+#             */
/*   Updated: 2024/09/10 12:33:18 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_appand(char *var, char *newstr)
{
	int		j;
	char	*path;

	j = -1;
	path = ft_expand_var(var);
	free(var);
	while (path && path[++j])
		newstr = apend_char_str(newstr, path[j]);
	return (newstr);
}
