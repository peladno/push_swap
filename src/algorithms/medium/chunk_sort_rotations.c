/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_rotations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 13:52:56 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/12 14:09:51 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rotate_a_to_pos(t_stack *a, int pos)
{
	int	steps;

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

void	rotate_b_to_target(t_stack *b, int target_pos)
{
	int	steps;

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
