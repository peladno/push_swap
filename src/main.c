/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:27:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/31 16:13:59 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack		*stack_a;
	t_strategy	strategy;
	t_node		*current_node;
	int			i;

	if (argc == 1)
		return (0);
	i = 1;
	stack_a = stack_init();
	if (!stack_a)
		return (1);
	strategy = STRATEGY_ADAPTIVE;
	if (parse_args(argc, argv, stack_a, &strategy) == 1)
	{
		ft_putstr_fd("Error\n", 2);
		stack_free(stack_a);
		return (1);
	}
	i = 0;
	current_node = stack_a->top;
	while (current_node)
	{
		printf("[%d] value=%d index=%d\n", i, current_node->value,
			current_node->index);
		current_node = current_node->next;
		i++;
	}
	stack_free(stack_a);
	return (0);
}
