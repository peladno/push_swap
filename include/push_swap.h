/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:04:43 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/30 18:05:39 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}					t_node;

typedef struct s_stack
{
	t_node			*top;
	t_node			*tail;
	int				size;
}					t_stack;

typedef enum e_strategy
{
	STRATEGY_ADAPTIVE,
	STRATEGY_SIMPLE,
	STRATEGY_MEDIUM,
	STRATEGY_COMPLEX
}					t_strategy;

t_stack				*stack_init(void);
void				stack_free(t_stack *stack_a);
int					stack_push_bottom(t_stack *stack_a, int value, int index);
int					coord_compress(t_stack *stack_a);
int					safe_atoi(const char *str, int *out_value);
int					validate_token(const char *tok, int *out_value);
void				free_split(char **tokens);
int					has_duplicates(t_stack *stack_a);
int					parse_args(int argc, char **argv, t_stack *stack_a,
						t_strategy *out_strategy);

#endif
