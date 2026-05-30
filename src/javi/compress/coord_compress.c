/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coord_compress.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:05:36 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/30 14:30:13 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	coord_compress(t_stack *stack)
{
	t_node *current;
	int *arr;

	if (!stack || stack->size == 0)
		return (1);
	arr = stack_to_array(stack);
	if (!arr)
		return (0);
	sort_int_array(arr, stack->size);
	current = stack->top;
	while (current)
	{
		current->index = find_index(arr, stack->size, current->value);
		current = current->next;
	}
	free(arr);
	return (1);
}