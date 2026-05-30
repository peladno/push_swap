/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:14:46 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/30 15:06:20 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// example ./push_swap --simple 1 209 -2 45 32 8

/*
 * TODO remaining in parse_args:
 *   ./push_swap --simple
 * - Call coordinate compression after stack A is fully built.
 * - Later: support quoted string input:
 *   ./push_swap "1 2 3"
 */

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

static int	parse_number(char *arg, t_stack *a)
{
	long	num;

	if (!is_number(arg))
		return (1);
	num = ft_atol(arg);
	if (num < INT_MIN || num > INT_MAX)
		return (1);
	if (has_duplicate(a, (int)num))
		return (1);
	if (!stack_add_back(a, (int)num))
		return (1);
	return (0);
}

static int	handle_strategy(char *arg, t_strategy *strategy, int *flag_count)
{
	if (!parse_strategy(arg, strategy))
		return (0);
	(*flag_count)++;
	if (*flag_count > 1)
		return (-1);
	return (1);
}

int	parse_args(int argc, char **argv, t_stack *a, t_strategy *strategy)
{
	int	i;
	int	flag_status;
	int	flag_count;

	i = 1;
	flag_count = 0;
	*strategy = STRATEGY_ADAPTIVE;
	if (argc == 1)
		return (0);
	while (i < argc)
	{
		flag_status = handle_strategy(argv[i], strategy, &flag_count);
		if (flag_status == -1 || (flag_status == 0 && parse_number(argv[i], a)))
		{
			free_stack(a);
			return (1);
		}
		i++;
	}
	return (0);
}
