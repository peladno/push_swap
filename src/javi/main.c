/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 22:19:54 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/28 22:27:21 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

// gcc -Wall -Wextra -Werror -I ./include ./src/javi/*.c -o push_swap
// ./push_swap

void	print_stack(t_stack *stack)
{
	t_node	*current;

	current = stack->top;
	while (current)
	{
		printf("value: %d | index: %d\n", current->value, current->index);
		current = current->next;
	}
	printf("size: %d\n", stack->size);
}

int	main(int argc, char **argv)
{
	t_stack a;
	t_strategy strategy;

	stack_init(&a);
	if (parse_args(argc, argv, &a, &strategy))
	{
		write(2, "Error\n", 6);
		// free_stack(&a);
		return (1);
	}
	print_stack(&a);
	// sfree_stack(&a);
	return (0);
}