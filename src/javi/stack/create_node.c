/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 21:54:13 by jperez-u          #+#    #+#             */
/*   Updated: 2026/05/28 22:20:36 by jperez-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*create_node(int value)
{
	t_node *new_node = malloc(sizeof(t_node));

	if (!new_node)
		return (NULL);

	new_node->value = value;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->index = -1;

	return (new_node);
}