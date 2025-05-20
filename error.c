/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:58 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:00 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	error_msg(char *msg)
{
	printf("%s\n", msg);
	perror(NULL);
	exit(EXIT_FAILURE);
}

void	error_free_msg(char *msg, t_table *table)
{
	printf("%s\n", msg);
	perror(NULL);
	if (table && table->th)
		join_philo_thread(table);
	destroy_all_mutex(table);
	free_all(table);
	exit(EXIT_FAILURE);
}

void	error_free_msg_nb(char *msg, t_table *table, int i)
{
	pthread_mutex_lock(&table->print);
	printf("%s pour philo id %d\n", msg, i);
	pthread_mutex_lock(&table->print);
	perror(NULL);
	if (table && table->th)
		join_philo_thread(table);
	destroy_all_mutex(table);
	free_all(table);
	exit(EXIT_FAILURE);
}
