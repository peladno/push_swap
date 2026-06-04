/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:02:28 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 21:38:48 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	push(t_stack *dst, t_stack *src)
{
	t_node	*node;

	if (!dst || !src || src->size == 0)
		return (OP_NOT_DONE);
	node = src->top;
	src->top = node->next;
	if (src->top)
		src->top->prev = NULL;
	else
		src->bottom = NULL;
	node->next = dst->top;
	node->prev = NULL;
	if (dst->top)
		dst->top->prev = node;
	else
		dst->bottom = node;
	dst->top = node;
	dst->size++;
	src->size--;
	return (OP_DONE);
}

void	pa(t_stack *a, t_stack *b)
{
	if (push(a, b) == STATUS_OK)
		ft_putstr("pa\n");
}

void	pb(t_stack *a, t_stack *b)
{
	if (push(b, a) == STATUS_OK)
		ft_putstr("pb\n");
}