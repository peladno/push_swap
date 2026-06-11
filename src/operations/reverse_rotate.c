/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/javi/operations/reverse_rotate.c
/*   Created: 2026/06/02 20:36:34 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/06 16:03:49 by jperez-u         ###   ########.fr       */
=======
/*   Created: 2026/06/04 21:08:32 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/10 18:33:04 by skusakab         ###   ########.fr       */
>>>>>>> master:src/operations/reverse_rotate.c
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	reverse_rotate(t_stack *stack)
{
	t_node	*old_bottom;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	old_bottom = stack->bottom;
	stack->bottom = old_bottom->prev;
	stack->bottom->next = NULL;
	old_bottom->prev = NULL;
	old_bottom->next = stack->top;
	stack->top->prev = old_bottom;
	stack->top = old_bottom;
	return (OP_DONE);
}

t_operation	rra(t_stack *stack_a)
{
<<<<<<< HEAD:src/javi/operations/reverse_rotate.c
	if (reverse_rotate(a) == OP_DONE)
		ft_putstr_fd("rra\n");
=======
	t_operation	result;

	result = reverse_rotate(stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("rra\n", 1);
	return (result);
>>>>>>> master:src/operations/reverse_rotate.c
}

t_operation	rrb(t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/reverse_rotate.c
	if (reverse_rotate(b) == OP_DONE)
		ft_putstr_fd("rrb\n");
=======
	t_operation	result;

	result = reverse_rotate(stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("rrb\n", 1);
	return (result);
>>>>>>> master:src/operations/reverse_rotate.c
}

t_operation	rrr(t_stack *stack_a, t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/reverse_rotate.c
	if (reverse_rotate(a) == OP_DONE && reverse_rotate(b) == OP_DONE)
		ft_putstr_fd("rrr\n");
=======
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	reverse_rotate(stack_a);
	reverse_rotate(stack_b);
	ft_putstr_fd("rrr\n", 1);
	return (OP_DONE);
>>>>>>> master:src/operations/reverse_rotate.c
}
