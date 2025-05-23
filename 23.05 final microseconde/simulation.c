/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:44 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:48 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	desynchro(t_table *table, t_philo *philo)
{
	if (table->odd_even == 2)
	{
		if (philo->id % 2 == 0)
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2);
		}
	}
	if (table->odd_even == 1)
	{
		if (philo->id % 2 != 0 && philo->id != 1)
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2 * 3);
		}
		if (philo->id % 2 == 0)
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2);
		}
	}
}

void	*start_rout(void *philo_ptr)
{
	t_table	*table;
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	table = philo->table;
	while (1)
	{
		pthread_mutex_lock(&table->eat_mutex);
		if (table->time_start != 0)
		{
			pthread_mutex_unlock(&table->eat_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->eat_mutex);
		ft_usleep(500);
	}
	desynchro(table, philo);
	routine(table, philo);
	return (NULL);
}

void	routine(t_table *table, t_philo *philo)
{
	while (check_flag_death(table) == 0)
	{
		if (philo->count_eat == table->nb_meal)
		{	
			pthread_mutex_lock(&table->eat_mutex);
			philo->h_of_die = get_time() + table->time_to_die;
			pthread_mutex_unlock(&table->eat_mutex);
			break ;
		}
		if (take_first_fork(table, philo) == 0)
			break ;
		if (take_second_fork(table, philo) == 0)
			break ;
		eating(table, philo);
		release_fork(philo);
		if (check_flag_death(table) != 0)
			break ;
		sleeping(table, philo);
		if (check_flag_death(table) != 0)
			break ;
		thinking(table, philo);
	}
}

void	start_simulation(t_table *table)
{
	int				i;
	long long int	start;

	while (check_flag_threads(table) != table->philo_nb)
	{
		ft_usleep(500);
	}
	start = get_time();
	i = 0;
	pthread_mutex_lock(&table->eat_mutex);
	table->time_start = start;
	pthread_mutex_unlock(&table->eat_mutex);
	while (i < table->philo_nb)
	{
		pthread_mutex_lock(&table->eat_mutex);
		table->philo[i].h_last_meal = start;
		table->philo[i].h_of_die = start + table->time_to_die;
		pthread_mutex_unlock(&table->eat_mutex);
		i++;
	}
}
