/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_in_routine_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:41 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:43:42 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	check_flag_death(t_table *table)
{
	int	death;

	pthread_mutex_lock(&table->death_mutex);
	death = table->flag_death;
	pthread_mutex_unlock(&table->death_mutex);
	return (death);
}

int	check_flag_threads(t_table *table)
{
	int	threads;

	pthread_mutex_lock(&table->synchro);
	threads = table->flag_thread;
	pthread_mutex_unlock(&table->synchro);
	return (threads);
}

long long int	check_h_of_die_i(t_table *table, int i)
{
	long long int	h_death;

	pthread_mutex_lock(&table->eat_mutex);
	h_death = table->philo[i].h_of_die;
	pthread_mutex_unlock(&table->eat_mutex);
	return (h_death);
}
