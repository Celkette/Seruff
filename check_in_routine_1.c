/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_in_routine_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:29 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:43:32 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	check_philo_death(t_table *table, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < table->philo_nb)
	{
		if (get_time() >= check_h_of_die_i(table, i))
		{
			pthread_mutex_lock(&table->death_mutex);
			table->flag_death = 1;
			pthread_mutex_unlock(&table->death_mutex);
			display_die_msg(table, &philo[i], "died\n");
			break ;
		}
		i++;
	}
}

void	check_count_meal(t_table *table)
{
	if (table->nb_meal != -1)
	{
		pthread_mutex_lock(&table->synchro);
		if (table->count_meal == (table->nb_meal * table->philo_nb))
		{
			pthread_mutex_unlock(&table->synchro);
			pthread_mutex_lock(&table->death_mutex);
			table->flag_death = 2;
			pthread_mutex_unlock(&table->death_mutex);
		}
		else
			pthread_mutex_unlock(&table->synchro);
	}
}

void	check_simulation(t_table *table)
{
	ft_usleep(table->time_to_die -2);
	while (check_flag_death(table) == 0)
	{
		ft_usleep(1);
		check_philo_death(table, table->philo);
		check_count_meal(table);
	}
	join_philo_thread(table);
	destroy_all_mutex(table);
	free_all(table);
}
