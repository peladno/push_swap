/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:04:43 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/12 14:34:29 by jperez-u         ###   ########.fr       */
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
void				chunk_sort(t_stack *a, t_stack *b);
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

void				radix_sort(t_stack *stack_a, t_stack *stack_b);

t_node				*find_x_below(t_stack *s, int x);
void				rotate_b_to_top(t_stack *b, t_node *target);
void				insertion_sort(t_stack *a, t_stack *b);

int					count_elements_in_chunk(t_stack *a, int chunk_idx,
						int chunk_size);
int					find_first_in_chunk_pos(t_stack *a, int lower, int upper);
int					get_dynamic_chunk_size(int n);
void				rotate_a_to_pos(t_stack *a, int pos);
void				rotate_b_to_target(t_stack *b, int target_pos);
void				align_stack_b(t_stack *b);
void				push_chunk_to_b(t_stack *a, t_stack *b, int c_idx,
						int c_size);

#endif
