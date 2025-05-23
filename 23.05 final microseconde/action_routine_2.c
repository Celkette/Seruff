/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_routine_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:12 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:43:14 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	eating(t_table *table, t_philo *philo)
{
	display_msg(table, philo, "is eating");
	pthread_mutex_lock(&table->eat_mutex);
	philo->h_last_meal = get_time();
	philo->h_of_die = philo->h_last_meal + table->time_to_die;
	philo->count_eat++;
	pthread_mutex_unlock(&table->eat_mutex);
	ft_usleep(table->time_to_eat);
	pthread_mutex_lock(&table->synchro);
	table->count_meal++;
	pthread_mutex_unlock(&table->synchro);
}

void	sleeping(t_table *table, t_philo *philo)
{
	display_msg(table, philo, "is sleeping");
	ft_usleep(table->time_to_sleep);
}

void	thinking(t_table *table, t_philo *philo)
{
	long long int	t_before_die;

	display_msg(table, philo, "is thinking");
	pthread_mutex_lock(&table->eat_mutex);
	t_before_die = philo->h_of_die - get_time();
	pthread_mutex_unlock(&table->eat_mutex);
	ft_usleep(t_before_die / 2);
}

void	display_msg(t_table *table, t_philo *philo, char *msg_action)
{
	if (check_flag_death(table))
		return ;
	pthread_mutex_lock(&table->print);
	printf("%lld %d %s\n", get_timestamp(table), philo->id, msg_action);
	pthread_mutex_unlock(&table->print);
}

void	display_die_msg(t_table *table, t_philo *philo, char *msg_action)
{
	pthread_mutex_lock(&table->print);
	printf("%lld %d %s\n", get_timestamp(table), philo->id, msg_action);
	pthread_mutex_unlock(&table->print);
}
