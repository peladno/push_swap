/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:14:46 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/30 18:05:24 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	parse_strategy(char *arg, t_strategy *strategy)
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

static int	check_empty_after_flag(int flag_count, t_stack *a)
{
	if (flag_count > 0 && a->size == 0)
	{
		error_msg();
		free_stack(a);
		return (1);
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_stack *a, t_strategy *strategy)
{
	int	i;
	int	flag_count;

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
				return (free_stack(a), 1);
		}
		else if (parse_string_arg(argv[i], a))
			return (free_stack(a), 1);
		i++;
	}
	return (check_empty_after_flag(flag_count, a));
}
