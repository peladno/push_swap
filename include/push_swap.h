/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 20:10:13 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/30 17:48:45 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <aio.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

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

int					parse_args(int argc, char **argv, t_stack *a,
						t_strategy *strategy);

void				error_msg(void);
int					ft_strcmp(const char *s1, const char *s2);
long				ft_atol(const char *nptr);
int					is_number(char *str);
t_node				*create_node(int data);
void				stack_init(t_stack *stack);
int					stack_add_back(t_stack *stack, int value);
int					has_duplicate(t_stack *a, int value);
void				free_stack(t_stack *stack);

int					coord_compress(t_stack *stack);
int					*stack_to_array(t_stack *stack);
void				sort_int_array(int *arr, int size);
int					find_index(int *arr, int size, int value);
char				**ft_split(char const *s, char c);
int					parse_string_arg(char *arg, t_stack *a);

#endif