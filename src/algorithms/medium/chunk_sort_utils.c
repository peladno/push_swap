/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 13:50:21 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/15 22:21:30 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	count_elements_in_chunk(t_stack *a, int chunk_idx, int chunk_size)
{
	t_node	*cur;
	int		lower;
	int		upper;
	int		count;

	lower = chunk_idx * chunk_size;
	upper = (chunk_idx + 1) * chunk_size;
	count = 0;
	cur = a->top;
	while (cur)
	{
		if (cur->index >= lower && cur->index < upper)
			count++;
		cur = cur->next;
	}
	return (count);
}

int	find_first_in_chunk_pos(t_stack *a, int lower, int upper)
{
	t_node	*cur;
	int		pos;

	cur = a->top;
	pos = 0;
	while (cur)
	{
		if (cur->index >= lower && cur->index < upper)
			return (pos);
		cur = cur->next;
		pos++;
	}
	return (-1);
}

int	get_dynamic_chunk_size(int n)
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
