/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:27:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/29 20:14:22 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char const *argv[])
{
	t_stack	*stack_a;
	t_node	*current_node;
	int		i;
	int		j;

	if (argc <= 1)
	{
		printf("Usage: %s <nbr> ...\n", argv[0]);
		return (1);
	}
	i = 1;
	stack_a = stack_init();
	while (i < argc)
	{
		stack_push_bottom(stack_a, ft_atoi(argv[i]), 0);
		i++;
	}
	coord_compress(stack_a);
	j = 0;
	current_node = stack_a->top;
	while (current_node)
	{
		printf("[%d] value=%d index=%d\n", j, current_node->value,
			current_node->index);
		current_node = current_node->next;
		j++;
	}
	stack_free(stack_a);
	return (0);
}
