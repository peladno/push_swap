/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:45:53 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 15:33:28 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	print_disorder(t_bench *bench)
{
	int	total;
	int	int_part;
	int	frac_part;

	total = (int)(bench->disorder * 10000 + 0.5);
	int_part = total / 100;
	frac_part = total % 100;
	if (frac_part < 10)
		ft_printf_err("[bench] disorder: %d.0%d%%\n", int_part, frac_part);
	else
		ft_printf_err("[bench] disorder: %d.%d%%\n", int_part, frac_part);
}

static int	count_total_ops(t_bench *bench)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (i < OP_COUNT)
	{
		result = result + bench->count[i];
		i++;
	}
	return (result);
}

void	bench_print(t_bench *bench)
{
	if (!bench->enabled)
		return ;
	print_disorder(bench);
	ft_printf_err("[bench] strategy:  %s\n", bench->strategy);
	ft_printf_err("[bench] total_ops:  %d\n", count_total_ops(bench));
	ft_printf_err("[bench] sa:  %d sb:  %d ss:  %d pa:  %d pb:  %d\n",
		bench->count[OP_SA], bench->count[OP_SB], bench->count[OP_SS],
		bench->count[OP_PA], bench->count[OP_PB]);
	ft_printf_err("[bench] ra:  %d rb:  %d rr:  %d  ", bench->count[OP_RA],
		bench->count[OP_RB], bench->count[OP_RR]);
	ft_printf_err("rra:  %d rrb:  %d rrr:  %d\n", bench->count[OP_RRA],
		bench->count[OP_RRB], bench->count[OP_RRR]);
}
