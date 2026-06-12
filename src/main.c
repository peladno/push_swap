/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:27:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/12 16:51:31 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	debug_print(t_stack *stack_a, t_stack *stack_b, char *label)
{
	t_node	*current_a;
	t_node	*current_b;
	int		i;

	i = 0;
	current_a = stack_a->top;
	current_b = stack_b->top;
	printf("--- %s (a:%d b:%d) ---\n", label, stack_a->size, stack_b->size);
	printf("\t[a]\t[b]\n");
	while (current_a || current_b)
	{
		if (current_a && current_b)
		{
			printf("[%d]\t %d\t %d\n", i, current_a->value, current_b->value);
			current_a = current_a->next;
			current_b = current_b->next;
		}
		else if (!current_a)
		{
			printf("[%d]\t \t %d\n", i, current_b->value);
			current_b = current_b->next;
		}
		else if (!current_b)
		{
			printf("[%d]\t %d\t \n", i, current_a->value);
			current_a = current_a->next;
		}
		i++;
	}
	printf("\n");
}

int	main(int argc, char **argv)
{
	t_stack		stack_a;
	t_stack		stack_b;
	t_config	config;

	if (argc == 1)
		return (0);
	stack_init(&stack_a);
	stack_init(&stack_b);
	if (parse_args(argc, argv, &stack_a, &config) == STATUS_ERROR)
	{
		ft_putstr_fd("Error\n", 2);
		stack_free(&stack_a);
		stack_free(&stack_b);
		return (1);
	}
	debug_print(&stack_a, &stack_b, "stack init");
	radix_sort(&stack_a, &stack_b);
	debug_print(&stack_a, &stack_b, "after radix");
	// sa(&stack_a);
	// debug_print(&stack_a, &stack_b, "after sa");
	// ra(&stack_a);
	// debug_print(&stack_a, &stack_b, "after ra");
	// rra(&stack_a);
	// debug_print(&stack_a, &stack_b, "after rra");
	// pb(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after pb");
	// pb(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after pb");
	// sb(&stack_b);
	// debug_print(&stack_a, &stack_b, "after sb");
	// ss(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after ss");
	// rb(&stack_b);
	// debug_print(&stack_a, &stack_b, "after rb");
	// rr(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after rr");
	// rrr(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after rrr");
	// pa(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b, "after pa");
	// ss(&stack_a, &stack_b);
	// debug_print(&stack_a, &stack_b,
	// 	"after ss with b size 1 (should not print 'ss')");
	stack_free(&stack_a);
	stack_free(&stack_b);
	return (0);
}
