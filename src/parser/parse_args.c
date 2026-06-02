/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:06:12 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/02 19:08:12 by skusakab         ###   ########.fr       */
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
		t_strategy *out_strategy)
{
	int	i;

	if (argc == 1)
		return (STATUS_OK);
	*out_strategy = STRATEGY_ADAPTIVE;
	i = 1;
	while (i < argc)
	{
		if (check_strategy(argv[i], out_strategy))
			;
		else if (push_arg_tokens(argv[i], stack_a) != STATUS_OK)
			return (STATUS_ERROR);
		i++;
	}
	if (has_duplicates(stack_a))
		return (STATUS_ERROR);
	coord_compress(stack_a);
	return (STATUS_OK);
}
