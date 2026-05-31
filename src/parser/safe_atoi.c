/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_atoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:01:10 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/30 17:01:25 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	safe_atoi(const char *str, int *out_value)
{
	long long	nbr;
	int			i;
	int			sign;
	int			limit_digit;

	nbr = 0;
	i = 0;
	sign = 1;
	limit_digit = 11;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
		limit_digit++;
	}
	while (ft_isdigit(str[i]) == 1 && i < limit_digit)
	{
		nbr = (nbr * 10) + (sign * (str[i] - '0'));
		i++;
	}
	if ((str[i] || (nbr < INT_MIN || INT_MAX < nbr)))
		return (1);
	*out_value = nbr;
	return (0);
}
