/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 17:19:02 by skusakab          #+#    #+#             */
/*   Updated: 2026/04/29 17:55:59 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d_tmp;
	unsigned char	*s_tmp;

	i = 0;
	d_tmp = (unsigned char *)dest;
	s_tmp = (unsigned char *)src;
	while (i < n)
	{
		d_tmp[i] = s_tmp[i];
		i++;
	}
	return (dest);
}
