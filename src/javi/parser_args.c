/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:14:46 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/28 21:01:07 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/push_swap.h"

// example ./push_swap --simple 1 209 -2 45 32 8

int	parse_strategy(char *arg, t_strategy *strategy)
{
	if (ft_strcmp(arg, "--simple") == 0)
		*strategy = STRATEGY_SIMPLE;
	else if (ft_strcmp(arg, "--medium") == 0)
		*strategy = STRATEGY_MEDIUM;
	else if (ft_strcmp(arg, "--complex") == 0)
		*strategy = STRATEGY_COMPLEX;
	else if (ft_strcmp(arg, "--adaptive") == 0)
		*strategy = STRATEGY_ADAPTIVE;
	else
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_stack *a, t_strategy *strategy)
{
	long	num;
	int		i;
	int		flag_count;

	i = 1;
	flag_count = 0;
	*strategy = STRATEGY_ADAPTIVE;
	if (argc == 1)
		return (0);
	while (i < argc)
	{
		if (parse_strategy(argv[i], strategy))
		{
			flag_count++;
			if (flag_count > 1)
				return (1);
		}
		else
		{
			if (!is_number(argv[i]))
				return (1);
			num = ft_atol(argv[i]);
			if (num < INT_MIN || num > INT_MAX)
				return (1);
			if (has_duplicate(a, (int)num)) // TODO
				return (1);
			if (!stack_add_back(a, (int)num)) // TODO
				return (1);
		}
		i++;
	}
	return (0);
}
