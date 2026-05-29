/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coordinate_compression.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 15:56:46 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/29 16:48:49 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	coord_compress(t_stack *stack_a)
{
	t_node	*target;
	t_node	*comp;
	size_t	counts;

	if (!stack_a || stack_a->size <= 1)
		return (1);
	target = stack_a->top;
	while (target)
	{
		comp = stack_a->top;
		counts = 0;
		while (comp)
		{
			if (target->value > comp->value)
				counts++;
			comp = comp->next;
		}
		target->index = counts;
		target = target->next;
	}
	return (0);
}
