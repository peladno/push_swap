/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/javi/operations/swap.c
/*   Created: 2026/05/31 15:39:59 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/06 16:02:24 by jperez-u         ###   ########.fr       */
=======
/*   Created: 2026/06/04 19:57:14 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/10 18:02:11 by skusakab         ###   ########.fr       */
>>>>>>> master:src/operations/swap.c
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*third;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	first = stack->top;
	second = first->next;
	third = second->next;
	second->prev = NULL;
	second->next = first;
	first->prev = second;
	if (third)
	{
		first->next = third;
		third->prev = first;
	}
	else
	{
		first->next = NULL;
		stack->bottom = first;
	}
	stack->top = second;
	return (OP_DONE);
}

t_operation	sa(t_stack *stack_a)
{
<<<<<<< HEAD:src/javi/operations/swap.c
	if (swap_top(a) == STATUS_OK)
		ft_putstr_fd("sa\n");
=======
	t_operation	result;

	result = swap(stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("sa\n", 1);
	return (result);
>>>>>>> master:src/operations/swap.c
}

t_operation	sb(t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/swap.c
	if (swap_top(b) == STATUS_OK)
		ft_putstr_fd("sb\n");
=======
	t_operation	result;

	result = swap(stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("sb\n", 1);
	return (result);
>>>>>>> master:src/operations/swap.c
}

t_operation	ss(t_stack *stack_a, t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/swap.c
	if (swap_top(a) == STATUS_OK && swap_top(b) == STATUS_OK)
		ft_putstr_fd("ss\n");
=======
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	swap(stack_a);
	swap(stack_b);
	ft_putstr_fd("ss\n", 1);
	return (OP_DONE);
>>>>>>> master:src/operations/swap.c
}
