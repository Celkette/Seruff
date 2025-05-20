/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:32 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:35 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	ft_usleep( size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

long long int	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
	{
		perror("gettimeofday failed");
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long long int	get_timestamp(t_table *table)
{
	long long	timestamp;

	timestamp = get_time() - table->time_start;
	return (timestamp);
}
