/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:06:12 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/12 16:52:59 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_strategy(char *str, t_config *config)
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

static int	is_bench_flag(char *str, t_config *config)
{
	if (ft_strncmp(str, "--bench", 8) == 0)
	{
		config->benched = 1;
		return (1);
	}
	return (0);
}

static t_status	process_one_token(char *token, t_stack *stack_a)
{
	int	value;

	if (validate_token(token, &value) != STATUS_OK)
		return (STATUS_ERROR);
	if (stack_push_bottom(stack_a, value) != STATUS_OK)
		return (STATUS_ERROR);
	return (STATUS_OK);
}

static t_status	push_arg_tokens(char *arg, t_stack *stack_a)
{
	int		i;
	char	**tokens;

	tokens = ft_split(arg, ' ');
	if (!tokens || !tokens[0])
	{
		free_split(tokens);
		return (STATUS_ERROR);
	}
	i = 0;
	while (tokens[i])
	{
		if (process_one_token(tokens[i], stack_a) != STATUS_OK)
		{
			free_split(tokens);
			return (STATUS_ERROR);
		}
		i++;
	}
	free_split(tokens);
	return (STATUS_OK);
}

t_status	parse_args(int argc, char **argv, t_stack *stack_a,
		t_config *config)
{
	int	i;
	int	strategy_count;
	int	bench_count;

	config->benched = 0;
	if (argc == 1)
		return (STATUS_OK);
	config->strategy = STRATEGY_ADAPTIVE;
	i = 1;
	strategy_count = 0;
	bench_count = 0;
	while (i < argc)
	{
		if (check_strategy(argv[i], config))
			strategy_count++;
		else if (is_bench_flag(argv[i], config))
			bench_count++;
		else if (push_arg_tokens(argv[i], stack_a) != STATUS_OK)
			return (STATUS_ERROR);
		i++;
	}
	if (strategy_count > 1 || bench_count > 1 || has_duplicates(stack_a))
		return (STATUS_ERROR);
	coord_compress(stack_a);
	return (STATUS_OK);
}
