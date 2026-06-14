/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 16:35:25 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 16:11:18 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_init(t_stack	*stack_a)
{
	stack_a->top = NULL;
	stack_a->bottom = NULL;
	stack_a->bench = NULL;
	stack_a->size = 0;
}
