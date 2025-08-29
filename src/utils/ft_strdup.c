/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:26:53 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/08 14:24:58 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	i;

	i = 0;
	dest = (char *)malloc(ft_strlen((char *)src) + 1);
	if (dest == NULL)
		return (NULL);
	while (src && src[i] != 0)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
	return (dest);
}
