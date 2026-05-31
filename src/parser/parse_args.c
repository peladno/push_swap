/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:06:12 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/31 16:04:11 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_strategy(char *str, t_strategy *out_strategy)
{
	if (ft_strncmp(str, "--simple", 9) == 0)
	{
		*out_strategy = STRATEGY_SIMPLE;
		return (1);
	}
	else if (ft_strncmp(str, "--medium", 9) == 0)
	{
		*out_strategy = STRATEGY_MEDIUM;
		return (1);
	}
	else if (ft_strncmp(str, "--complex", 10) == 0)
	{
		*out_strategy = STRATEGY_COMPLEX;
		return (1);
	}
	else if (ft_strncmp(str, "--adaptive", 11) == 0)
	{
		*out_strategy = STRATEGY_ADAPTIVE;
		return (1);
	}
	return (0);
}

static int	push_arg_tokens(char *arg, t_stack *stack_a)
{
	int		i;
	int		value;
	char	**tokens;

	tokens = ft_split(arg, ' ');
	if (!tokens || !tokens[0])
	{
		free_split(tokens);
		return (1);
	}
	i = 0;
	while (tokens[i])
	{
		if (validate_token(tokens[i], &value) == 1)
		{
			free_split(tokens);
			return (1);
		}
		stack_push_bottom(stack_a, value, 0);
		i++;
	}
	free_split(tokens);
	return (0);
}

int	parse_args(int argc, char **argv, t_stack *stack_a,
		t_strategy *out_strategy)
{
	int	i;

	if (argc == 1)
		return (0);
	*out_strategy = STRATEGY_ADAPTIVE;
	i = 1;
	while (i < argc)
	{
		if (check_strategy(argv[i], out_strategy) == 1)
			;
		else if (push_arg_tokens(argv[i], stack_a) == 1)
			return (1);
		i++;
	}
	if (has_duplicates(stack_a) == 1)
		return (1);
	coord_compress(stack_a);
	return (0);
}
