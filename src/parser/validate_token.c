/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 15:05:30 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/16 20:40:41 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_status	safe_atoi(const char *str, int *out_value)
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
		sign = -1;
		i++;
		limit_digit++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]) && i < limit_digit)
		nbr = (nbr * 10) + (sign * (str[i++] - '0'));
	if (str[i] || nbr < INT_MIN || INT_MAX < nbr)
		return (STATUS_ERROR);
	*out_value = (int)nbr;
	return (STATUS_OK);
}

t_status	validate_token(const char *token, int *out_value)
{
	int	i;

	i = 0;
	if (!token)
		return (STATUS_ERROR);
	if (token[i] == '-' || token[i] == '+')
		i++;
	if (ft_isdigit(token[i]) == 0)
		return (STATUS_ERROR);
	return (safe_atoi(token, out_value));
}
