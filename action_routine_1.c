/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_routine_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:42:12 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:42:16 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	take_first_fork(t_table *table, t_philo *philo)
{
	if (table->philo_nb == 1)
	{
		pthread_mutex_lock(philo->r_fork);
		display_msg(table, philo, "has taken a fork");
		pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	if (philo->id == 1)
		pthread_mutex_lock(philo->l_fork);
	else
		pthread_mutex_lock(philo->r_fork);
	display_msg(table, philo, "has taken a fork");
	if (check_flag_death(table) != 0)
	{
		if (philo->id == 1)
			pthread_mutex_unlock(philo->l_fork);
		else
			pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	return (1);
}

int	take_second_fork(t_table *table, t_philo *philo)
{
	if (check_flag_death(table) != 0)
	{
		if (philo->id == 1)
			pthread_mutex_unlock(philo->l_fork);
		else
			pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	if (philo->id == 1)
		pthread_mutex_lock(philo->r_fork);
	else
		pthread_mutex_lock(philo->l_fork);
	display_msg(table, philo, "has taken a fork");
	if (check_flag_death(table))
	{		
		release_fork(philo);
		return (0);
	}
	return (2);
}

void	release_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}
