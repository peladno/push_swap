/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:04:43 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/11 17:43:50 by skusakab         ###   ########.fr       */
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
	t_node			*bottom;
	int				size;
}					t_stack;

typedef enum e_strategy
{
	STRATEGY_ADAPTIVE,
	STRATEGY_SIMPLE,
	STRATEGY_MEDIUM,
	STRATEGY_COMPLEX
}					t_strategy;

typedef enum e_status
{
	STATUS_OK,
	STATUS_ERROR
}					t_status;

typedef enum e_operation
{
	OP_NOT_DONE,
	OP_DONE
}					t_operation;

t_node				*create_node(int value);
void				stack_init(t_stack *stack_a);
void				stack_free(t_stack *stack_a);
t_status			stack_push_bottom(t_stack *stack_a, int value);
void				coord_compress(t_stack *stack_a);
t_status			validate_token(const char *tok, int *out_value);
void				free_split(char **tokens);
int					has_duplicates(t_stack *stack_a);
t_status			parse_args(int argc, char **argv, t_stack *stack_a,
						t_strategy *out_strategy);
t_operation			sa(t_stack *stack_a);
t_operation			sb(t_stack *stack_b);
t_operation			ss(t_stack *stack_a, t_stack *stack_b);
t_operation			pa(t_stack *stack_a, t_stack *stack_b);
t_operation			pb(t_stack *stack_a, t_stack *stack_b);
t_operation			ra(t_stack *stack_a);
t_operation			rb(t_stack *stack_b);
t_operation			rr(t_stack *stack_a, t_stack *stack_b);
t_operation			rra(t_stack *stack_a);
t_operation			rrb(t_stack *stack_b);
t_operation			rrr(t_stack *stack_a, t_stack *stack_b);
double				compute_disorder(t_stack *stack_a);

#endif
