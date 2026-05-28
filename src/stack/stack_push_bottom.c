/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_push_bottom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:00:19 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/28 20:30:35 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	stack_push_bottom(t_stack *s, int value, int index)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (1);
	new_node->value = value;
	new_node->index = index;
	new_node->next = NULL;
	if (s->size == 0)
	{
		new_node->prev = NULL;
		s->top = new_node;
		s->tail = new_node;
	}
	else
	{
		new_node->prev = s->tail;
		s->tail->next = new_node;
		s->tail = new_node;
	}
	s->size++;
	return (0);
}
