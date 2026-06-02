/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_push_bottom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:00:19 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/02 17:42:00 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_status	stack_push_bottom(t_stack *stack_a, int value, int index)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (STATUS_ERROR);
	new_node->value = value;
	new_node->index = index;
	new_node->next = NULL;
	if (stack_a->size == 0)
	{
		new_node->prev = NULL;
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
