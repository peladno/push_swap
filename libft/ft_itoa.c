/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 18:14:59 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/08 14:57:09 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	callen(int n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

static void	dectoa(long n, size_t len, char *dest)
{
	if (n == 0)
		dest[0] = '0';
	dest[len] = '\0';
	while (n > 0)
	{
		dest[--len] = n % 10 + '0';
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*tmp;
	size_t	len;
	long	longn;

	len = callen(n);
	longn = (long)n;
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	if (n < 0)
	{
		longn *= -1;
		tmp[0] = '-';
	}
	dectoa(longn, len, tmp);
	return (tmp);
}
