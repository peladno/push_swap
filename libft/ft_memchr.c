/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 20:50:12 by skusakab          #+#    #+#             */
/*   Updated: 2026/04/29 21:32:58 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*s_tmp;
	unsigned char	c_tmp;

	s_tmp = (unsigned char *)s;
	c_tmp = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (s_tmp[i] == c_tmp)
			return (&s_tmp[i]);
		i++;
	}
	return (NULL);
}
