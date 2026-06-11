/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/javi/operations/rotate.c
/*   Created: 2026/06/01 21:51:19 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/06 16:03:22 by jperez-u         ###   ########.fr       */
=======
/*   Created: 2026/06/04 20:48:00 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/10 18:01:47 by skusakab         ###   ########.fr       */
>>>>>>> master:src/operations/rotate.c
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "push_swap.h"

static t_operation	rotate(t_stack *stack)
{
	t_node	*old_top;

	if (!stack || stack->size < 2)
		return (OP_NOT_DONE);
	old_top = stack->top;
	stack->top = old_top->next;
	stack->top->prev = NULL;
	stack->bottom->next = old_top;
	old_top->prev = stack->bottom;
	stack->bottom = old_top;
	old_top->next = NULL;
	return (OP_DONE);
}

t_operation	ra(t_stack *stack_a)
{
<<<<<<< HEAD:src/javi/operations/rotate.c
	if (rotate(a) == OP_DONE)
		ft_putstr_fd("ra\n");
=======
	t_operation	result;

	result = rotate(stack_a);
	if (result == OP_DONE)
		ft_putstr_fd("ra\n", 1);
	return (result);
>>>>>>> master:src/operations/rotate.c
}

t_operation	rb(t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/rotate.c
	if (rotate(b) == OP_DONE)
		ft_putstr_fd("rb\n");
=======
	t_operation	result;

	result = rotate(stack_b);
	if (result == OP_DONE)
		ft_putstr_fd("rb\n", 1);
	return (result);
>>>>>>> master:src/operations/rotate.c
}

t_operation	rr(t_stack *stack_a, t_stack *stack_b)
{
<<<<<<< HEAD:src/javi/operations/rotate.c
	if (rotate(a) == OP_DONE && rotate(b) == OP_DONE)
		ft_putstr_fd("rr\n");
=======
	if (!stack_a || !stack_b || stack_a->size < 2 || stack_b->size < 2)
		return (OP_NOT_DONE);
	rotate(stack_a);
	rotate(stack_b);
	ft_putstr_fd("rr\n", 1);
	return (OP_DONE);
>>>>>>> master:src/operations/rotate.c
}
