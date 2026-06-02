/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 14:08:29 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/08 14:56:23 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	putdigits(long l, int fd)
{
	if (l > 9)
		putdigits(l / 10, fd);
	ft_putchar_fd('0' + (l % 10), fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	l;

	l = (long)n;
	if (l < 0)
	{
		ft_putchar_fd('-', fd);
		l = -l;
	}
	putdigits(l, fd);
}
