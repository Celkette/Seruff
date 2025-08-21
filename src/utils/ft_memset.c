/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:17:44 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/16 13:13:52 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

//@Rremplit la zone mémoire pointée par s avec la valeur c sur n octets.
// @param s Pointeur vers la zone mémoire à remplir
// @param c Valeur à remplir dans la zone mémoire
// @param n Nombre d'octets à remplir
// @return Pointeur vers la zone mémoire remplie
void	*ft_memset(void *s, int c, size_t n)
{
	size_t			index;
	unsigned char	*mem;

	mem = (unsigned char *)s;
	index = 0;
	while (index < n)
		mem[index++] = (unsigned char)c;
	return (s);
}
