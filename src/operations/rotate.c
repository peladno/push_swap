/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:48:00 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 16:37:37 by skusakab         ###   ########.fr       */
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
	{
		if (stack_a->bench)
			stack_a->bench->count[OP_RA]++;
		ft_putstr_fd("ra\n", 1);
	}
	return (result);
}

t_operation	rb(t_stack *stack_b)
{
	t_operation	result;

	result = rotate(stack_b);
	if (result == OP_DONE)
	{
		if (stack_b->bench)
			stack_b->bench->count[OP_RB]++;
		ft_putstr_fd("rb\n", 1);
	}
	return (result);
}

t_operation	rr(t_stack *stack_a, t_stack *stack_b)
{
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	rotate(stack_a);
	rotate(stack_b);
	if (stack_a->bench)
		stack_a->bench->count[OP_RR]++;
	ft_putstr_fd("rr\n", 1);
	return (OP_DONE);
}
