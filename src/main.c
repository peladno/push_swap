/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:27:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/02 20:03:37 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack		stack_a;
	t_strategy	strategy;
	t_node		*current_node;
	int			i;

	if (argc == 1)
		return (0);
	i = 1;
	stack_init(&stack_a);
	strategy = STRATEGY_ADAPTIVE;
	if (parse_args(argc, argv, &stack_a, &strategy) == STATUS_ERROR)
	{
		ft_putstr_fd("Error\n", 2);
		stack_free(&stack_a);
		return (1);
	}
	i = 0;
	current_node = stack_a.top;
	while (current_node)
	{
		printf("[%d] value=%d index=%d\n", i, current_node->value,
			current_node->index);
		current_node = current_node->next;
		i++;
	}
	stack_free(&stack_a);
	return (0);
}
