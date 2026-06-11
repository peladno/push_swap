/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radix_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 20:10:15 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/10 21:29:42 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	count_bit(int size)
{
	int	bit;

	bit = 0;
	while ((1 << bit) < size)
		bit++;
	return (bit);
}

static void	process_bit(t_stack *stack_a, t_stack *stack_b, int bit)
{
	int	i;
	int	size;

	i = 0;
	size = stack_a->size;
	while (i < size)
	{
		if (((stack_a->top->index >> bit) & 1) == 0)
			pb(stack_a, stack_b);
		else
			ra(stack_a);
		i++;
	}
	while (stack_b->size != 0)
		pa(stack_a, stack_b);
}

void	radix_sort(t_stack *stack_a, t_stack *stack_b)
{
	int	bit;
	int	max_bit;

	if (stack_a->size < 2)
		return ;
	bit = 0;
	max_bit = count_bit(stack_a->size);
	while (bit < max_bit)
	{
		process_bit(stack_a, stack_b, bit);
		bit++;
	}
}
