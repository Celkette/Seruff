/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:55 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:56 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	alloc_other(t_table *table)
{
	table->th = malloc(sizeof(pthread_t) * table->philo_nb);
	if (!table->th)
		error_free_msg("Error: malloc pthread_t thread failed", table);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	if (!table->fork)
		error_free_msg("Error: malloc pthread_mutex_t fork failed", table);
	table->philo = malloc(sizeof(t_philo) * (table->philo_nb));
	if (!table->philo)
		error_free_msg("Error: malloc t_philo philo failed", table);
}

t_table	*init_simulation(int argc, char **argv)
{
	t_table	*table;

	table = init_table(argc, argv);
	alloc_other(table);
	init_print(table);
	init_forks(table);
	init_philo(table);
	init_other_mutex(table);
	return (table);
}
