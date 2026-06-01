/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 22:19:54 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 20:12:06 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>
#include <unistd.h>

// gcc -Wall -Wextra -Werror
//
// 	-I ./include ./src/javi/compress/*.c ./src/javi/stack/*.c ./src/javi/parser/*.c ./src/javi/utils/*.c
// -o push_swap
// ./push_swap

void	print_stack(t_stack *stack)
{
	t_node	*current;

	current = stack->top;
	printf("----- STACK -----\n");
	while (current)
	{
		printf("value: %d | index: %d\n", current->value, current->index);
		current = current->next;
	}
	printf("size: %d\n", stack->size);
	printf("-----------------\n");
}

int	main(int argc, char **argv)
{
	t_stack		a;
	t_strategy	strategy;

	stack_init(&a);
	if (parse_args(argc, argv, &a, &strategy) == 1)
	{
		free_stack(&a);
		return (1);
	}
	// if (is_sorted(&a))
	// {
	// 	printf("Stack already sorted.\n");
	// 	free_stack(&a);
	// 	return (0);
	// }
	if (!coord_compress(&a))
	{
		write(2, "Compress Error\n", 15);
		free_stack(&a);
		return (1);
	}
	print_stack(&a);
	free_stack(&a);
	return (0);
}
