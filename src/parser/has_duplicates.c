/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_duplicates.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:26:41 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/30 16:49:31 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	has_duplicates(t_stack *stack_a)
{
	t_node	*target;
	t_node	*comp;

	if (!stack_a)
		return (1);
	target = stack_a->top;
	while (target)
	{
		comp = target->next;
		while (comp)
		{
			if (target->value == comp->value)
				return (1);
			comp = comp->next;
		}
		target = target->next;
	}
	return (0);
}
