/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 21:51:19 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/06 16:03:22 by jperez-u         ###   ########.fr       */
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
	old_top->next = NULL;
	stack->bottom = old_top;
	return (OP_DONE);
}

void	ra(t_stack *a)
{
	if (rotate(a) == OP_DONE)
		ft_putstr_fd("ra\n");
}

void	rb(t_stack *b)
{
	if (rotate(b) == OP_DONE)
		ft_putstr_fd("rb\n");
}

void	rr(t_stack *a, t_stack *b)
{
	if (rotate(a) == OP_DONE && rotate(b) == OP_DONE)
		ft_putstr_fd("rr\n");
}
