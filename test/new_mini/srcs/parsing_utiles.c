/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:28:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/06/13 15:29:42 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int ft_strisalnum(char *str)
{
    while (*str)
    {
        if (!isalnum(*str))
            return (0);
        str++;
    }
    return (1);
}
