/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:36:34 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/04 21:11:13 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	rra(t_stack *a)
{
	if (reverse_rotate(a) == OP_DONE)
		ft_putstr("rra\n");
}

void	rrb(t_stack *b)
{
	if (reverse_rotate(b) == OP_DONE)
		ft_putstr("rrb\n");
}

void	rrr(t_stack *a, t_stack *b)
{
	if (reverse_rotate(a) == OP_DONE && reverse_rotate(b) == OP_DONE)
		ft_putstr("rrr\n");
}
