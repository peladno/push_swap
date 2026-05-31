/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:02:28 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/31 17:52:05 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// pa (push a): Take the first element at the top of b and put it at the top of a.
// Do nothing if b is empty.

// pb (push b): Take the first element at the top of a and put it at the top of b.
// Do nothing if a is empty.

#include "push_swap.h"

static int	push(t_stack *a, t_stack *b)
{
	t_node	*firstb;
	t_node	*secondb;

	if (!a || b->size == 0)
		return ;
}
