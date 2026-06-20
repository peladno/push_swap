/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:06:12 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/20 15:21:55 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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

static t_status	process_args(char *arg, t_stack *stack_a, t_config *config)
{
	if (check_strategy(arg, config))
		config->strategy_count++;
	else if (is_bench_flag(arg))
		config->benched++;
	/* LIVE --silent: else if (is_silent_flag(arg)) config->silent = 1; */
	else if (push_arg_tokens(arg, stack_a) != STATUS_OK)
		return (STATUS_ERROR);
	return (STATUS_OK);
}

t_status	parse_args(int argc, char **argv, t_stack *stack_a,
		t_config *config)
{
	int	i;

	config->benched = 0;
	config->strategy_count = 0;
	/* LIVE --silent: config->silent = 0; */
	if (argc == 1)
		return (STATUS_OK);
	config->strategy = STRATEGY_ADAPTIVE;
	i = 1;
	while (i < argc)
	{
		if (process_args(argv[i], stack_a, config) != STATUS_OK)
			return (STATUS_ERROR);
		i++;
	}
	if (config->benched > 1 || config->strategy_count > 1
		|| has_duplicates(stack_a))
		return (STATUS_ERROR);
	coord_compress(stack_a);
	return (STATUS_OK);
}
