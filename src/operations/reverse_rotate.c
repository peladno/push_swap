/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 21:08:32 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 14:34:03 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	reverse_rotate(t_stack *stack)
{
	t_node	*old_bottom;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	old_bottom = stack->bottom;
	stack->bottom = old_bottom->prev;
	stack->bottom->next = NULL;
	old_bottom->prev = NULL;
	old_bottom->next = stack->top;
	stack->top->prev = old_bottom;
	stack->top = old_bottom;
	return (OP_DONE);
}

t_operation	rra(t_stack *stack_a)
{
	t_operation	result;

	result = reverse_rotate(stack_a);
	if (result == OP_DONE)
		emit(stack_a->bench, OP_RRA, "rra\n");
	return (result);
}

t_operation	rrb(t_stack *stack_b)
{
	t_operation	result;

	result = reverse_rotate(stack_b);
	if (result == OP_DONE)
		emit(stack_b->bench, OP_RRB, "rrb\n");
	return (result);
}

t_operation	rrr(t_stack *stack_a, t_stack *stack_b)
{
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	reverse_rotate(stack_a);
	reverse_rotate(stack_b);
	emit(stack_a->bench, OP_RRR, "rrr\n");
	return (OP_DONE);
}
