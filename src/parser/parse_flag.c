/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 15:11:48 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 15:21:14 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	check_strategy(char *str, t_config *config)
{
	if (ft_strncmp(str, "--simple", 9) == 0)
	{
		config->strategy = STRATEGY_SIMPLE;
		return (1);
	}
	else if (ft_strncmp(str, "--medium", 9) == 0)
	{
		config->strategy = STRATEGY_MEDIUM;
		return (1);
	}
	else if (ft_strncmp(str, "--complex", 10) == 0)
	{
		config->strategy = STRATEGY_COMPLEX;
		return (1);
	}
	else if (ft_strncmp(str, "--adaptive", 11) == 0)
	{
		config->strategy = STRATEGY_ADAPTIVE;
		return (1);
	}
	return (0);
}

int	is_bench_flag(char *str)
{
	if (ft_strncmp(str, "--bench", 8) == 0)
		return (1);
	return (0);
}
