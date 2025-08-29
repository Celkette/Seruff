/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 22:00:15 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/25 22:21:57 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static size_t	ft_intlen(int n)
{
	if (n / 10 == 0)
		return (1);
	return (ft_intlen(n / 10) + 1);
}

char	*ft_itoa(int n)
{
	char	*r;
	int		sign;
	size_t	size;

	sign = n;
	if (n < 0)
		size = ft_intlen(n) + 2;
	else
		size = ft_intlen(n) + 1;
	r = malloc(sizeof(char) * size);
	if (r == NULL)
		return (NULL);
	r[--size] = '\0';
	while (size > 0)
	{
		if (n < 0)
			r[--size] = (n % 10) * -1 + '0';
		else
			r[--size] = n % 10 + '0';
		n /= 10;
	}
	if (sign < 0)
		r[0] = '-';
	return (r);
}
