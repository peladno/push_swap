/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:39:59 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 21:36:22 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	swap_top(t_stack *stack)
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
	first->next = third;
	if (third != NULL)
		third->prev = first;
	else
		stack->bottom = first;
	stack->top = second;
	return (OP_DONE);
}

void	sa(t_stack *a)
{
	if (swap_top(a) == STATUS_OK)
		ft_putstr("sa\n");
}

void	sb(t_stack *b)
{
	if (swap_top(b) == STATUS_OK)
		ft_putstr("sb\n");
}

void	ss(t_stack *a, t_stack *b)
{
	if (swap_top(a) == STATUS_OK && swap_top(b) == STATUS_OK)
		ft_putstr("ss\n");
}
