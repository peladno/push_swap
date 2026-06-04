/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:01:35 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/04 22:08:09 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_operation	push(t_stack *stack_from, t_stack *stack_to)
{
	t_node	*from_first;

	if (stack_from->size == 0)
		return (OP_NOT_DONE);
	from_first = stack_from->top;
	stack_from->top = from_first->next;
	if (stack_from->top)
		stack_from->top->prev = NULL;
	else
		stack_from->bottom = NULL;
	stack_from->size--;
	from_first->next = stack_to->top;
	if (stack_to->top)
		stack_to->top->prev = from_first;
	else
		stack_to->bottom = from_first;
	stack_to->top = from_first;
	stack_to->size++;
	return (OP_DONE);
}

t_operation	pa(t_stack *stack_a, t_stack *stack_b)
{
	t_operation	result;

	result = push(stack_b, stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("pa\n", 1);
	return (result);
}

t_operation	pb(t_stack *stack_a, t_stack *stack_b)
{
	t_operation	result;

	result = push(stack_a, stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("pb\n", 1);
	return (result);
}
