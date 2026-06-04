/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:48:00 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/04 23:05:11 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_operation	rotate(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*last;

	if (stack->size < 2)
		return (OP_NOT_DONE);
	first = stack->top;
	second = first->next;
	last = stack->bottom;
	first->prev = last;
	first->next = NULL;
	second->prev = NULL;
	last->next = first;
	stack->top = second;
	stack->bottom = first;
	return (OP_DONE);
}

t_operation	ra(t_stack *stack_a)
{
	t_operation	result;

	result = rotate(stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("ra\n", 1);
	return (result);
}

t_operation	rb(t_stack *stack_b)
{
	t_operation	result;

	result = rotate(stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("rb\n", 1);
	return (result);
}

t_operation	rr(t_stack *stack_a, t_stack *stack_b)
{
	if (stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	rotate(stack_a);
	rotate(stack_b);
	ft_putstr_fd("rr\n", 1);
	return (OP_DONE);
}
