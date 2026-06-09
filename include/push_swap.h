/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:04:43 by skusakab          #+#    #+#             */
/*   Updated: 2026/06/09 21:14:05 by jperez-u         ###   ########.fr       */
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

typedef struct s_bench
{
	long			total_ops;

	long			sa;
	long			sb;
	long			ss;
	long			pa;
	long			pb;
	long			ra;
	long			rb;
	long			rr;
	long			rra;
	long			rrb;
	long			rrr;

	char			*strategy;
	int				n;
	long			rotations;
	long			inserts;
	long			max_depth;

}					t_bench;

int					parse_args(int argc, char **argv, t_stack *a,
						t_strategy *strategy);
void				free_split(char **tokens);
int					coord_compress(t_stack *stack);
t_status			validate_token(const char *token, int *out_value);
int					has_duplicates(t_stack *stack_a);

t_node				*create_node(int data);
void				stack_init(t_stack *stack);
void				stack_free(t_stack *stack_a);
t_status			stack_push_bottom(t_stack *stack_a, int value);

void				sa(t_stack *a);
void				sb(t_stack *b);
void				ss(t_stack *a, t_stack *b);

void				pa(t_stack *a, t_stack *b);
void				pb(t_stack *a, t_stack *b);

void				rra(t_stack *a);
void				rrb(t_stack *b);
void				rrr(t_stack *a, t_stack *b);

void				ra(t_stack *a);
void				rb(t_stack *b);
void				rr(t_stack *a, t_stack *b);

t_node				*find_x_below(t_stack *s, int x);
void				rotate_b_to_top(t_stack *b, t_node *target);
void				insertion_sort(t_stack *a, t_stack *b);

#endif
