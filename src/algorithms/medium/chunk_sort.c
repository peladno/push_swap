/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/06/11 22:04:17 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/11 22:04:17 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"


static int	count_elements_in_chunk(t_stack *a, int chunk_idx, int chunk_size)
{
	t_node *cur;
	int lower = chunk_idx * chunk_size;
	int upper = (chunk_idx + 1) * chunk_size;
	int count = 0;

	cur = a->top;
	while (cur)
	{
		if (cur->index >= lower && cur->index < upper)
			count++;
		cur = cur->next;
	}
	return (count);
}

static int	find_first_in_chunk_pos(t_stack *a, int lower, int upper)
{
	t_node *cur = a->top;
	int pos = 0;

	while (cur)
	{
		if (cur->index >= lower && cur->index < upper)
			return (pos);
		cur = cur->next;
		pos++;
	}
	return (-1);
}

static void	rotate_a_to_pos(t_stack *a, int pos)
{
	int steps;

	if (pos <= 0 || a->size < 2)
		return ;
	if (pos <= a->size / 2)
	{
		while (pos-- > 0)
			ra(a);
	}
	else
	{
		steps = a->size - pos;
		while (steps-- > 0)
			rra(a);
	}
}

static int	get_target_position_b(t_stack *b, int idx)
{
	t_node *cur;
	int pos;
	int best;
	int best_pos;
	int max;
	int max_pos;

	if (b->size == 0)
		return (0);
	cur = b->top;
	pos = 0;
	best = -1;
	best_pos = -1;
	max = -1;
	max_pos = 0;
	while (cur)
	{
		if (cur->index > max)
			max = cur->index, max_pos = pos;
		if (cur->index < idx && cur->index > best)
			best = cur->index, best_pos = pos;
		cur = cur->next;
		pos++;
	}
	return (best_pos != -1 ? best_pos : max_pos);
}

static void	rotate_b_to_target(t_stack *b, int target_pos)
{
	int steps;

	if (b->size < 2 || target_pos <= 0)
		return ;
	if (target_pos <= b->size / 2)
	{
		while (target_pos-- > 0)
			rb(b);
	}
	else
	{
		steps = b->size - target_pos;
		while (steps-- > 0)
			rrb(b);
	}
}

static int	get_max_pos_b(t_stack *b)
{
	t_node *cur;
	int max_index;
	int max_pos;
	int pos;

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

void	align_stack_b(t_stack *b)
{
	int max_pos;

	if (b->size < 2)
		return ;
	max_pos = get_max_pos_b(b);
	rotate_b_to_target(b, max_pos);
}

static void	push_chunk_to_b(t_stack *a, t_stack *b, int chunk_idx,
		int chunk_size)
{
	int pos_in_a;
	int target_pos;
	int remaining;
	int lower;
	int upper;

	remaining = count_elements_in_chunk(a, chunk_idx, chunk_size);
	lower = chunk_idx * chunk_size;
	upper = (chunk_idx + 1) * chunk_size;
	while (remaining-- > 0)
	{
		pos_in_a = find_first_in_chunk_pos(a, lower, upper);
		rotate_a_to_pos(a, pos_in_a);
		target_pos = get_target_position_b(b, a->top->index);
		rotate_b_to_target(b, target_pos);
		pb(a, b);
	}
}

static int	get_dynamic_chunk_size(int n)
{
	if (n <= 20)
		return (4);
	else if (n <= 100)
		return (15);
	else if (n <= 500)
		return (30);
	else
		return (45);
}

void	chunk_sort(t_stack *a, t_stack *b)
{
	int n;
	int chunk_size;
	int num_chunks;
	int chunk_idx;

	if (!a || !b || a->size <= 1)
		return ;
	n = a->size;
	chunk_size = get_dynamic_chunk_size(n);
	num_chunks = (n + chunk_size - 1) / chunk_size;
	chunk_idx = 0;
	while (chunk_idx < num_chunks)
	{
		push_chunk_to_b(a, b, chunk_idx, chunk_size);
		chunk_idx++;
	}
	align_stack_b(b);
	while (b->size > 0)
		pa(a, b);
}
