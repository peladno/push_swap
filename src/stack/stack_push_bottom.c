/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_push_bottom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:00:19 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/02 19:30:21 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_status	stack_push_bottom(t_stack *stack_a, int value)
{
	t_node	*new_node;

	new_node = create_node(value);
	if (!new_node)
		return (STATUS_ERROR);
	if (stack_a->size == 0)
	{
		stack_a->top = new_node;
		stack_a->bottom = new_node;
	}
	else
	{
		new_node->prev = stack_a->bottom;
		stack_a->bottom->next = new_node;
		stack_a->bottom = new_node;
	}
	stack_a->size++;
	return (STATUS_OK);
}
