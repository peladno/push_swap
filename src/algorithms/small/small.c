/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 12:45:44 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 14:03:42 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_two(t_stack *stack_a)
{
	if (stack_a->top->index > stack_a->bottom->index)
		sa(stack_a);
}

static void	sort_three(t_stack *a)
{
	int	x;
	int	y;
	int	z;

	x = a->top->index;
	y = a->top->next->index;
	z = a->top->next->next->index;
	if (x > y && y < z && x < z)
		sa(a);
	else if (x > y && y > z)
	{
		sa(a);
		rra(a);
	}
	else if (x > y && y < z && x > z)
		ra(a);
	else if (x < y && y > z && x < z)
	{
		sa(a);
		ra(a);
	}
	else if (x < y && y > z && x > z)
		rra(a);
}

static int	min_pos(t_stack *stack_a)
{
	int		min_index;
	int		min_pos;
	int		pos;
	t_node	*cur;

	min_index = stack_a->top->index;
	min_pos = 0;
	pos = 0;
	cur = stack_a->top;
	while (cur)
	{
		if (cur->index < min_index)
		{
			min_index = cur->index;
			min_pos = pos;
		}
		cur = cur->next;
		pos++;
	}
	return (min_pos);
}

void	sort_small(t_stack *stack_a, t_stack *stack_b)
{
	int	size;

	size = stack_a->size;
	if (4 <= size && size <= 5)
	{
		rotate_a_to_pos(stack_a, min_pos(stack_a));
		pb(stack_a, stack_b);
		rotate_a_to_pos(stack_a, min_pos(stack_a));
		pb(stack_a, stack_b);
	}
	if (size == 2 || size == 4)
		sort_two(stack_a);
	if (size == 3 || size == 5)
		sort_three(stack_a);
	pa(stack_a, stack_b);
	pa(stack_a, stack_b);
	stack_a->bench->strategy = "Adaptive / O(1)";
}
