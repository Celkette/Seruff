/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:47:43 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/14 07:06:02 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			i;
	int				last;
	unsigned char	uc;

	if (!s)
		return (0);
	uc = (unsigned char)c;
	i = 0;
	last = -1;
	while (s[i])
		if ((unsigned char)s[i++] == uc)
			last = i - 1;
	if (last != -1)
		return ((char *)(s + last));
	if ((unsigned char)s[i] == uc)
		return ((char *)(s + i));
	return (NULL);
}
