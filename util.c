/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:27 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:30 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	result;
	int	i;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		result = result * 10 +(nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
