/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:38:56 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/27 11:49:31 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < n && s[i] != '\0')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return ((char *)res);
}
/*
VERSION qui retourne NULL si n == 0;
LE VRAI STRNDUP RENVOIE UNE CHAINE VIDE

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*res;

	if (!s || n <= 0)
		return (NULL);
	if ((size_t)ft_strlen((char *)s) < n)
		n = (size_t)ft_strlen((char *)s);
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return ((char *)res);
}
	*/
