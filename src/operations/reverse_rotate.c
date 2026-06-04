/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 21:08:32 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/04 23:04:42 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_operation	reverse_rotate(t_stack *stack)
{
	t_node	*first;
	t_node	*last;
	t_node	*pre_last;

	if (stack->size < 2)
		return (OP_NOT_DONE);
	first = stack->top;
	last = stack->bottom;
	pre_last = last->prev;
	last->prev = NULL;
	last->next = first;
	first->prev = last;
	pre_last->next = NULL;
	stack->top = last;
	stack->bottom = pre_last;
	return (OP_DONE);
}

t_operation	rra(t_stack *stack_a)
{
	t_operation	result;

	result = reverse_rotate(stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("rra\n", 1);
	return (result);
}

t_operation	rrb(t_stack *stack_b)
{
	t_operation	result;

	result = reverse_rotate(stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("rrb\n", 1);
	return (result);
}

t_operation	rrr(t_stack *stack_a, t_stack *stack_b)
{
	if (stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	reverse_rotate(stack_a);
	reverse_rotate(stack_b);
	ft_putstr_fd("rrr\n", 1);
	return (OP_DONE);
}
