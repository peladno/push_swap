/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:36:13 by skusakab          #+#    #+#             */
/*   Updated: 2026/05/19 17:14:43 by skusakab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	countwords(char const *s, char c)
{
	size_t	i;
	size_t	counts;

	i = 0;
	counts = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			counts++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (counts);
}

static size_t	get_words_len(char const *s, char c)
{
	size_t	counts;

	counts = 0;
	while (s[counts] && s[counts] != c)
		counts++;
	return (counts);
}

static char	*alloc_word(char const *s, size_t len)
{
	char	*word;
	size_t	i;

	i = 0;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**cleanup(char **array, size_t i)
{
	while (i--)
		free(array[i]);
	free(array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	len;
	size_t	i;

	array = malloc(sizeof(char *) * (countwords(s, c) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		len = get_words_len(s, c);
		array[i] = alloc_word(s, len);
		if (!array[i])
			return (cleanup(array, i));
		i++;
		s += len;
	}
	array[i] = NULL;
	return (array);
}
