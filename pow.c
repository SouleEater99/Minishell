/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pow.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:24:18 by aelkheta          #+#    #+#             */
/*   Updated: 2024/06/26 11:57:50 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

double myPow(double x, int n) 
{
    if (n == 0) 
    {
        return 1.0;
    }
    else if (n < 0)
    {
        x = 1 / x;
        if (n == -2147483648)
        {
            n = 2147483647;
            return x * myPow(x, n / 2);
        }
        n = -n;
    }
    double res = myPow(x, n / 2);
    if (n % 2)
        return (res * res * x);
    else
        return (res * res);
}

int main()
{
    printf("%lf\n", myPow(2.1, 3));
}