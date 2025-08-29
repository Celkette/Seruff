/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 22:00:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/12 12:19:06 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*c_dest;
	unsigned char	*c_src;
	size_t			index;

	if (dest == NULL && src == NULL)
		return (NULL);
	c_dest = (unsigned char *)dest;
	c_src = (unsigned char *)src;
	index = 0;
	while (index < n)
	{
		c_dest[index] = c_src[index];
		index++;
	}
	return (dest);
}
