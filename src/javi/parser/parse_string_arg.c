/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string_arg.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:37:17 by jperez-u          #+#    #+#             */
/*   Updated: 2026/06/01 20:20:25 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	parse_number_arg(char *arg, t_stack *a)
{
	long	num;

	num = ft_atol(arg);
	if (!is_number(arg))
	{
		error_msg();
		return (1);
	}
	if (has_duplicate(a, (int)num))
	{
		error_msg();
		return (1);
	}
	if (!stack_add_back(a, (int)num))
	{
		error_msg();
		return (1);
	}
	return (0);
}

static int	parse_tokens(char **tokens, t_stack *a)
{
	int	i;

	i = 0;
	if (!tokens || !tokens[0])
		return (1);
	while (tokens[i])
	{
		if (parse_number_arg(tokens[i], a))
			return (1);
		i++;
	}
	return (0);
}

static void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

int	parse_string_arg(char *arg, t_stack *a)
{
	char	**tokens;

	tokens = ft_split(arg, ' ');
	if (!tokens)
		return (1);
	if (parse_tokens(tokens, a))
	{
		free_tokens(tokens);
		return (1);
	}
	free_tokens(tokens);
	return (0);
}
