/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 14:22:02 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/09 21:12:48 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	insert_all_from_a_to_b(t_stack *a, t_stack *b)
{
	t_node	*x;
	t_node	*x_below;

	pb(a, b);
	while (a->size > 0)
	{
		x = a->top;
		x_below = find_x_below(b, x->index);
		rotate_b_to_top(b, x_below);
		pb(a, b);
	}
}

static t_node	*find_max_node(t_stack *b)
{
	t_node	*cur;
	t_node	*max_node;

	cur = b->top;
	max_node = cur;
	while (cur)
	{
		if (cur->index > max_node->index)
			max_node = cur;
		cur = cur->next;
	}
	return (max_node);
}

static void	finalize_b(t_stack *b)
{
	t_node	*max_node;

	max_node = find_max_node(b);
	rotate_b_to_top(b, max_node);
}

static void	return_all_to_a(t_stack *a, t_stack *b)
{
	while (b->size > 0)
		pa(a, b);
}

void	insertion_sort(t_stack *a, t_stack *b)
{
	if (a->size < 2)
		return ;
	insert_all_from_a_to_b(a, b);
	finalize_b(b);
	return_all_to_a(a, b);
}

// #include <stdio.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	t_stack	a;
// 	t_stack	b;
// 	t_node	*cur;

// 	a.top = NULL;
// 	a.bottom = NULL;
// 	a.size = 0;
// 	b.top = NULL;
// 	b.bottom = NULL;
// 	b.size = 0;
// 	push_top(&a, new_node(3, 2));
// 	push_top(&a, new_node(1, 0));
// 	push_top(&a, new_node(5, 4));
// 	push_top(&a, new_node(2, 1));
// 	push_top(&a, new_node(4, 3));
// 	insertion_sort(&a, &b);
// 	cur = a.top;
// 	while (cur)
// 	{
// 		printf("%d ", cur->value);
// 		cur = cur->next;
// 	}
// 	printf("\n");
// 	return (0);
// }
