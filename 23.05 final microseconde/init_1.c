/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:58 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:45:13 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

t_table	*init_table(int argc, char**argv)
{
	t_table	*table;

	table = malloc(sizeof (t_table));
	if (!table)
		error_msg("Error: malloc failed");
	table->philo_nb = ft_atol(argv[1]);
	check_odd_even(table->philo_nb, table);
	table->time_to_die = ft_atol(argv[2]) * 1000;
	table->time_to_eat = ft_atol(argv[3]) * 1000;
	table->time_to_sleep = ft_atol(argv[4]) * 1000;
	table->nb_meal = -1;
	if (argc == 6)
		table->nb_meal = ft_atol(argv[5]);
	table->count_meal = 0;
	table->time_start = 0;
	table->actual_ms = 0;
	table->flag_death = 0;
	table->th = NULL;
	table->fork = NULL;
	table->philo = NULL;
	return (table);
}

void	init_philo(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nb)
	{
		table->philo[i].id = i + 1;
		table->philo[i].r_fork = &table->fork[i];
		if (i == 0)
			table->philo[i].l_fork = &table->fork[table->philo_nb - 1];
		else
			table->philo[i].l_fork = &table->fork[i - 1];
		table->philo[i].h_last_meal = 0;
		table->philo[i].t_since_last_meal = 0;
		table->philo[i].h_of_die = 2000000000000000;
		table->philo[i].count_eat = 0;
		table->philo[i].table = table;
		i++;
	}
}

void	init_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_mutex_init(&table->fork[i], NULL))
			error_free_msg_nb("Error: init_fork mutex failed", table, i);
		i++;
	}
}

void	init_print(t_table *table)
{
	if (pthread_mutex_init(&table->print, NULL))
		error_free_msg("Error: init_print mutex failed", table);
}

void	init_other_mutex(t_table *table)
{
	if (pthread_mutex_init(&table->eat_mutex, NULL))
		error_free_msg("Error: init_eat_mutex failed", table);
	if (pthread_mutex_init(&table->synchro, NULL))
		error_free_msg("Error: init_flag_thread_mutex failed", table);
	if (pthread_mutex_init(&table->death_mutex, NULL))
		error_free_msg("Error: init_death_mutex failed", table);
}
