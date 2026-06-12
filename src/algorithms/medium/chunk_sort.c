/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:05:37 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/12 14:30:15 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	chunk_sort(t_stack *a, t_stack *b)
{
	int	n;
	int	chunk_size;
	int	num_chunks;
	int	chunk_idx;

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
