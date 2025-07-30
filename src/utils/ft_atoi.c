/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:45:36 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/08 18:03:02 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	get_num(const char *nptr)
{
	int	i;
	int	r;

	i = 0;
	r = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		r *= 10;
		r += (char)nptr[i++] - '0';
	}
	return (r);
}

int	ft_atoi(const char *nptr)
{
	int	sign;

	if (!nptr)
		return (-1);
	while (*nptr == ' ' || *nptr == '\n' || *nptr == '\f' || *nptr == '\r' \
		|| *nptr == '\t' || *nptr == '\v')
		nptr++;
	sign = 1;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	return (get_num(nptr) * sign);
}
