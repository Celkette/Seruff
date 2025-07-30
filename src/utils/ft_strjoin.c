/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:10:16 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/27 11:50:54 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*news;
	size_t	len_s1;
	size_t	len_s2;

	len_s1 = 0;
	len_s2 = 0;
	/*if (!s1 || !s2)//VOIR WSI NECESSAIRE
		return (NULL);*/
	news = malloc(sizeof(char)
			* (ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1));
	if (!news)
		return (NULL);
	while (s1 && s1[len_s1] != '\0')//correction while (s1 &&
	{
		news[len_s1] = s1[len_s1];
		len_s1++;
	}
	while (s2 && s2[len_s2] != '\0')//correction while (s2 &&
	{
		news[len_s1 + len_s2] = s2[len_s2];
		len_s2++;
	}
	news[len_s1 + len_s2] = '\0';
	return (news);
}
//Alloue (avec malloc(3)) et retourne une nouvelle
//chaîne, résultat de la concaténation de s1 et s2