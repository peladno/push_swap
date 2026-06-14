/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy_label.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 17:46:46 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 17:59:23 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	*strategy_label(t_strategy requested, t_strategy chosen)
{
	if (chosen == STRATEGY_ADAPTIVE)
		return (NULL);
	if (requested == STRATEGY_ADAPTIVE)
	{
		if (chosen == STRATEGY_SIMPLE)
			return ("Adaptive / O(n²)");
		if (chosen == STRATEGY_MEDIUM)
			return ("Adaptive / O(n√n)");
		if (chosen == STRATEGY_COMPLEX)
			return ("Adaptive / O(n log n)");
	}
	if (requested == STRATEGY_SIMPLE)
		return ("Simple / O(n²)");
	if (requested == STRATEGY_MEDIUM)
		return ("Medium / O(n√n)");
	if (requested == STRATEGY_COMPLEX)
		return ("Complex / O(n log n)");
	return (NULL);
}
