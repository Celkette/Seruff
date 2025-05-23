/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:48 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:43:50 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	destroy_all_mutex(t_table *table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	if (table->fork)
	{
		while (i < table->philo_nb)
		{
			if (pthread_mutex_destroy(&table->fork[i]))
				error_msg("Error: destroy mutex fork failed\n");
			i++;
		}
	}
	if (pthread_mutex_destroy(&table->print))
		error_msg("Error: destroy mutex print failed\n");
	if (pthread_mutex_destroy(&table->eat_mutex))
		error_msg("Error: destroy eat_mutex failed\n");
	if (pthread_mutex_destroy(&table->synchro))
		error_msg("Error: destroy synchro_mutex failed\n");
	if (pthread_mutex_destroy(&table->death_mutex))
		error_msg("Error: destroy death_mutex failed\n");
}

void	free_all(t_table *table)
{
	if (table)
	{
		if (table->fork)
			free(table->fork);
		if (table->th)
			free(table->th);
		if (table->philo)
			free(table->philo);
		free(table);
	}
}
