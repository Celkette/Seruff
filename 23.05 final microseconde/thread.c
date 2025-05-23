/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:37 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:40 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	creat_philo_thread(t_table *table)
{
	int	i;

	i = 0;
	table->flag_thread = 0;
	while (i < table->philo_nb)
	{
		if (pthread_create(&table->th[i], NULL, start_rout, &table->philo[i]))
		{
			error_free_msg_nb("Error: create_thread philo failed", table, i);
		}
		pthread_mutex_lock(&table->synchro);
		table->flag_thread++;
		pthread_mutex_unlock(&table->synchro);
		i++;
	}
}

void	join_philo_thread(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_join(table->th[i], NULL))
			error_free_msg_nb("Error: join_thread philo failed", table, i);
		i++;
	}
}
