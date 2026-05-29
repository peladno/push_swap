/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 19:59:36 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/06 21:39:22 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tmp;
	size_t	s1_len;
	size_t	s2_len;
	size_t	tmp_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	tmp_len = s1_len + s2_len;
	tmp = malloc(sizeof(char) * (tmp_len + 1));
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, s1, s1_len);
	ft_memcpy(tmp + s1_len, s2, s2_len);
	tmp[tmp_len] = '\0';
	return (tmp);
}
