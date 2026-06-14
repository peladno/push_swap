/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 18:10:04 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 18:52:39 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_strategy	decide_sort(t_strategy requested, double disorder)
{
	if (requested != STRATEGY_ADAPTIVE)
		return (requested);
	else
	{
		if (disorder < 0.2)
			return (STRATEGY_SIMPLE);
		else if (disorder < 0.5)
			return (STRATEGY_MEDIUM);
		else
			return (STRATEGY_COMPLEX);
	}
}

static void	run_sort(t_stack *stack_a, t_stack *stack_b, t_strategy chosen)
{
	if (!chosen)
		return ;
	if (chosen == STRATEGY_SIMPLE)
		insertion_sort(stack_a, stack_b);
	if (chosen == STRATEGY_MEDIUM)
		chunk_sort(stack_a, stack_b);
	if (chosen == STRATEGY_COMPLEX)
		radix_sort(stack_a, stack_b);
}

void	dispatch(t_stack *stack_a, t_stack *stack_b, t_strategy requested)
{
	t_strategy	chosen;
	double		disorder;

	disorder = stack_a->bench->disorder;
	chosen = decide_sort(requested, disorder);
	stack_a->bench->strategy = strategy_label(requested, chosen);
	run_sort(stack_a, stack_b, chosen);
}
