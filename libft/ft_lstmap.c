/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 18:19:25 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/10 19:36:33 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	try_map_one(t_list **result, void *old_content, void *(*f)(void *),
		void (*del)(void *))
{
	t_list	*new;
	void	*new_content;

	new_content = f(old_content);
	if (!new_content)
	{
		ft_lstclear(result, del);
		return (1);
	}
	new = ft_lstnew(new_content);
	if (!new)
	{
		del(new_content);
		ft_lstclear(result, del);
		return (1);
	}
	ft_lstadd_back(result, new);
	return (0);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;

	if (!lst || !f || !del)
		return (NULL);
	result = NULL;
	while (lst)
	{
		if (try_map_one(&result, lst->content, f, del) == 1)
			return (NULL);
		lst = lst->next;
	}
	return (result);
}
