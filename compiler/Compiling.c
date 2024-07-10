/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziz <aziz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:11:45 by aziz              #+#    #+#             */
/*   Updated: 2024/05/31 06:19:52 by aziz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void compiling()
{
    int i = 0;
    while(i < 4)
    {
        write(1, "\rC             ", 16);
        usleep(100000);
        write(1, "\rCo            ", 16);
        usleep(100000);
        write(1, "\rCom           ", 16);
        usleep(100000);
        write(1, "\rComp          ", 16);
        usleep(100000);
        write(1, "\rCompi         ", 16);
        usleep(100000);
        write(1, "\rCompil        ", 16);
        usleep(100000);
        write(1, "\rCompili       ", 16);
        usleep(100000);
        write(1, "\rCompilin      ", 16);
        usleep(100000);
        write(1, "\rCompiling     ", 16);
        usleep(100000);
        write(1, "\rCompiling  .  ", 16);
        usleep(100000);
        write(1, "\rCompiling  .. ", 16);
        usleep(100000);
        write(1, "\rCompiling  ...", 16);
        usleep(100000);
        write(1, "\r ompiling  ...", 16);
        usleep(100000);
        write(1, "\r  mpiling  ...", 16);
        usleep(100000);
        write(1, "\r   piling  ...", 16);
        usleep(100000);
        write(1, "\r    iling  ...", 16);
        usleep(100000);
        write(1, "\r     ling  ...", 16);
        usleep(100000);
        write(1, "\r      ing  ...", 16);
        usleep(100000);
        write(1, "\r       ng  ...", 16);
        usleep(100000);
        write(1, "\r        g  ...", 16);
        usleep(100000);
        write(1, "\r           ...", 16);
        usleep(100000);
        write(1, "\r            ..", 16);
        usleep(100000);
        write(1, "\r             .", 16);
        usleep(100000);
        write(1, "\r              ", 16);
        i++;
    }
    write(1, "\rcompiling done\n", 17);
}

void cleaning()
{
    int i = 0;
    while(i < 1)
    {
        write(1, "\rC             ", 16);
        usleep(100000);
        write(1, "\rCl            ", 16);
        usleep(100000);
        write(1, "\rCle           ", 16);
        usleep(100000);
        write(1, "\rClea          ", 16);
        usleep(100000);
        write(1, "\rClean         ", 16);
        usleep(100000);
        write(1, "\rCleani        ", 16);
        usleep(100000);
        write(1, "\rCleanin       ", 16);
        usleep(100000);
        write(1, "\rCleaning      ", 16);
        usleep(100000);
        write(1, "\rCleaning   .  ", 16);
        usleep(100000);
        write(1, "\rCleaning   .. ", 16);
        usleep(100000);
        write(1, "\rCleaning   ...", 16);
        usleep(100000);
        write(1, "\r leaning   ...", 16);
        usleep(100000);
        write(1, "\r  eaning   ...", 16);
        usleep(100000);
        write(1, "\r   aning   ...", 16);
        usleep(100000);
        write(1, "\r    ning   ...", 16);
        usleep(100000);
        write(1, "\r     ing   ...", 16);
        usleep(100000);
        write(1, "\r      ng   ...", 16);
        usleep(100000);
        write(1, "\r       g   ...", 16);
        usleep(100000);
        write(1, "\r           ...", 16);
        usleep(100000);
        write(1, "\r            ..", 16);
        usleep(100000);
        write(1, "\r             .", 16);
        usleep(100000);
        write(1, "\r              ", 16);
        i++;
    }
    write(1, "\rcleaning done\n", 16);
}

int main()
{
    compiling();
    cleaning();
}
