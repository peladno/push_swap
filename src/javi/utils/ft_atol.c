/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 20:21:14 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 20:19:59 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	safe_atol(const char *str, long *num)
{
	long	result;
	int		sign;
	int		digit;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (sign == 1 && result > (INT_MAX - digit) / 10)
			return (1);
		if (sign == -1 && result > ((long)INT_MAX + 1 - digit) / 10)
			return (1);
		result = result * 10 + digit;
		str++;
	}
	*num = result * sign;
	return (0);
}
