/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:02:28 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 21:19:40 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// pa (push a): Take the first element at the top of b and put it at the top of a.
// Do nothing if b is empty.

// pb (push b): Take the first element at the top of a and put it at the top of b.
// Do nothing if a is empty.

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
