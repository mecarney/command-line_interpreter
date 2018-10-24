/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitspaces.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 22:24:09 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/22 15:18:42 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_letters(char const *str)
{
	size_t	count;

	count = 0;
	while (str[count] && str[count] != ' '
		&& str[count] != '\t' && str[count] != '\v')
		count++;
	return (count);
}

static size_t	ft_count_words(char const *str)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\v')
		{
			if (*(str + 1) == ' ' || *(str + 1) == '\t'
				|| *(str + 1) == '\v' || *(str + 1) == '\0')
				count++;
		}
		str++;
	}
	return (count);
}

char			**ft_strsplitspaces(char const *s)
{
	size_t		i;
	size_t		j;
	size_t		words;
	char		**tab;

	i = 0;
	if (!s)
		return (NULL);
	words = ft_count_words(s);
	if (!(tab = (char **)malloc(sizeof(char *) * (words + 1))))
		return (NULL);
	while (i < words)
	{
		while (*s && (*s == ' ' || *s == '\t' || *s == '\v'))
			s++;
		tab[i] = (char *)malloc(sizeof(char) * (ft_count_letters(s) + 1));
		j = 0;
		while (*s && (*s != ' ' && *s != '\t' && *s != '\v'))
			tab[i][j++] = *s++;
		tab[i][j] = '\0';
		i++;
	}
	tab[i] = 0;
	return (tab);
}
