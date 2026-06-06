/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 14:22:02 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/06 17:33:31 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_node	*find_x_below(t_stack *stack, int x_index)
{
	t_node	*cur;
	t_node	*best;
	int		max_index;

	cur = stack->top;
	best = NULL;
	max_index = -1;
	while (cur)
	{
		if (cur->index < x_index)
		{
			if (!best || cur->index > best->index)
				best = cur;
		}
		if (cur->index > max_index)
			max_index = cur->index;
		cur = cur->next;
	}
	if (best)
		return (best);
	cur = stack->top;
	while (cur)
	{
		if (cur->index == max_index)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

// static t_node	*find_x_below(t_stack *s, int x)
// {
// 	t_node	*cur;
// 	t_node	*best;
// 	int		max;

// 	cur = s->top;
// 	best = NULL;
// 	max = -1;
// 	while (cur)
// 	{
// 		if (cur->index < x && (!best || cur->index > best->index))
// 			best = cur;
// 		if (cur->index > max)
// 			max = cur->index;
// 		cur = cur->next;
// 	}
// 	if (best)
// 		return (best);
// 	cur = s->top;
// 	while (cur)
// 	{
// 		if (cur->index == max)
// 			return (cur);
// 		cur = cur->next;
// 	}
// 	return (NULL);
// }

static int	get_pos(t_stack *s, t_node *target)
{
	// TODO
}

static void	rotate_b_to_top(t_stack *b, t_node *target)
{
	// TODO
}

void	insertion_sort(t_stack *a, t_stack *b, t_bench *bench)
{
	// TODO
	pb(a, b);
}
