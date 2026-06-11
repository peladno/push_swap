/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_disorder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 17:29:20 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/11 17:53:34 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	compute_disorder(t_stack *stack_a)
{
	t_node	*node_i;
	t_node	*node_j;
	int		mistakes;
	int		total_pairs;

	if (!stack_a || stack_a->size < 2)
		return (0.0);
	node_i = stack_a->top;
	mistakes = 0;
	total_pairs = 0;
	while (node_i)
	{
		node_j = node_i->next;
		while (node_j)
		{
			if (node_i->value > node_j->value)
				mistakes++;
			total_pairs++;
			node_j = node_j->next;
		}
		node_i = node_i->next;
	}
	return ((double)mistakes / total_pairs);
}
