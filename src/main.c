/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:27:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/14 18:57:00 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack		stack_a;
	t_stack		stack_b;
	t_config	config;
	t_bench		bench;

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
	stack_a.bench = &bench;
	stack_b.bench = &bench;
	bench_init(&bench, config.benched, compute_disorder(&stack_a));
	/* LIVE --silent: bench.silent = config.silent; */
	dispatch(&stack_a, &stack_b, config.strategy);
	bench_print(&bench);
	/* LIVE --silent: when silent, print only the op total to stdout —
	   if (config.silent) { ft_putnbr_fd(<sum of bench.count[]>, 1);
	       ft_putchar_fd('\n', 1); }
	   (make count_total_ops non-static + prototype it, or sum inline) */
	stack_free(&stack_a);
	stack_free(&stack_b);
	return (0);
}
