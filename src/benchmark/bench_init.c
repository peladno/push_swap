/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:45:57 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/12 14:59:17 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	bench_init(t_bench *bench, int enabled, double disorder)
{
	ft_bzero(bench->count, sizeof(int) * OP_COUNT);
	bench->strategy = NULL;
	bench->enabled = enabled;
	bench->disorder = disorder;
}
