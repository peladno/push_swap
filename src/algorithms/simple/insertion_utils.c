/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 21:11:17 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/09 21:12:08 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*find_x_below(t_stack *s, int x)
{
	t_node	*cur;
	t_node	*best;
	int		max;

	cur = s->top;
	best = NULL;
	max = -1;
	while (cur)
	{
		if (cur->index < x && (!best || cur->index > best->index))
			best = cur;
		if (cur->index > max)
			max = cur->index;
		cur = cur->next;
	}
	if (best)
		return (best);
	cur = s->top;
	while (cur)
	{
		if (cur->index == max)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

void	rotate_b_to_top(t_stack *b, t_node *target)
{
	t_node	*cur;
	int		pos;
	int		size;

	if (!b || !target || b->top == target)
		return ;
	cur = b->top;
	pos = 0;
	while (cur && cur != target)
	{
		cur = cur->next;
		pos++;
	}
	if (!cur)
		return ;
	size = b->size;
	if (pos < size / 2)
		while (pos--)
			rb(b);
	else
	{
		pos = size - pos;
		while (pos--)
			rrb(b);
	}
}
