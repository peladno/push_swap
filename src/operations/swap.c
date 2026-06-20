/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 19:57:14 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 14:36:25 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*third;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	first = stack->top;
	second = first->next;
	third = second->next;
	second->prev = NULL;
	second->next = first;
	first->prev = second;
	if (third)
	{
		first->next = third;
		third->prev = first;
	}
	else
	{
		first->next = NULL;
		stack->bottom = first;
	}
	stack->top = second;
	return (OP_DONE);
}

t_operation	sa(t_stack *stack_a)
{
	t_operation	result;

	result = swap(stack_a);
	if (result == OP_DONE)
		emit(stack_a->bench, OP_SA, "sa\n");
	return (result);
}

t_operation	sb(t_stack *stack_b)
{
	t_operation	result;

	result = swap(stack_b);
	if (result == OP_DONE)
		emit(stack_b->bench, OP_SB, "sb\n");
	return (result);
}

t_operation	ss(t_stack *stack_a, t_stack *stack_b)
{
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	swap(stack_a);
	swap(stack_b);
	emit(stack_a->bench, OP_SS, "ss\n");
	return (OP_DONE);
}
