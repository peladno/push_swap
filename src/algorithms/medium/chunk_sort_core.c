/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:30:04 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/12 14:38:32 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_max_pos_b(t_stack *b)
{
	t_node	*cur;
	int		max_index;
	int		max_pos;
	int		pos;

	max_index = -1;
	max_pos = 0;
	pos = 0;
	cur = b->top;
	while (cur)
	{
		if (cur->index > max_index)
		{
			max_index = cur->index;
			max_pos = pos;
		}
		cur = cur->next;
		pos++;
	}
	return (max_pos);
}

static int	get_best_lower_pos_b(t_stack *b, int idx)
{
	t_node	*cur;
	int		pos;
	int		best_pos;
	int		best_index;

	best_pos = -1;
	best_index = -1;
	pos = 0;
	cur = b->top;
	while (cur)
	{
		if (cur->index < idx && cur->index > best_index)
		{
			best_index = cur->index;
			best_pos = pos;
		}
		cur = cur->next;
		pos++;
	}
	return (best_pos);
}

static int	get_target_position_b(t_stack *b, int idx)
{
	int	best_pos;

	if (b->size == 0)
		return (0);
	best_pos = get_best_lower_pos_b(b, idx);
	if (best_pos != -1)
		return (best_pos);
	return (get_max_pos_b(b));
}

void	align_stack_b(t_stack *b)
{
	int	max_pos;

	if (b->size < 2)
		return ;
	max_pos = get_max_pos_b(b);
	rotate_b_to_target(b, max_pos);
}

void	push_chunk_to_b(t_stack *a, t_stack *b, int c_idx, int c_size)
{
	int	pos_in_a;
	int	target_pos;
	int	remaining;
	int	lower;
	int	upper;

	remaining = count_elements_in_chunk(a, c_idx, c_size);
	lower = c_idx * c_size;
	upper = (c_idx + 1) * c_size;
	while (remaining-- > 0)
	{
		pos_in_a = find_first_in_chunk_pos(a, lower, upper);
		rotate_a_to_pos(a, pos_in_a);
		target_pos = get_target_position_b(b, a->top->index);
		rotate_b_to_target(b, target_pos);
		pb(a, b);
	}
}
