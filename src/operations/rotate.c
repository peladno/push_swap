/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:48:00 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 14:42:14 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	rotate(t_stack *stack)
{
	t_node	*old_top;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	old_top = stack->top;
	stack->top = old_top->next;
	stack->top->prev = NULL;
	stack->bottom->next = old_top;
	old_top->prev = stack->bottom;
	stack->bottom = old_top;
	old_top->next = NULL;
	return (OP_DONE);
}

t_operation	ra(t_stack *stack_a)
{
	t_operation	result;

	result = rotate(stack_a);
	if (result == OP_DONE)
		emit(stack_a->bench, OP_RA, "ra\n");
	return (result);
}

t_operation	rb(t_stack *stack_b)
{
	t_operation	result;

	result = rotate(stack_b);
	if (result == OP_DONE)
		emit(stack_b->bench, OP_RB, "rb\n");
	return (result);
}

t_operation	rr(t_stack *stack_a, t_stack *stack_b)
{
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	rotate(stack_a);
	rotate(stack_b);
	emit(stack_a->bench, OP_RR, "rr\n");
	return (OP_DONE);
}
