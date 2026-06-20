/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 14:13:20 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 14:22:51 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	emit(t_bench *bench, t_op_id op_id, char *name)
{
	bench->count[op_id]++;
	ft_putstr_fd(name, 1);
}
