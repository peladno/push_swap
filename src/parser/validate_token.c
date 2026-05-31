/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 15:05:30 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/31 15:12:19 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	validate_token(const char *token, int *out_value)
{
	int	i;

	i = 0;
	if (!token)
		return (1);
	if (token[i] == '-')
		i++;
	if (ft_isdigit(token[i]) == 0)
		return (1);
	return (safe_atoi(token, out_value));
}
