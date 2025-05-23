/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:44:51 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:52 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	main(int argc, char **argv)
{
	t_table	*table;

	check_args_1(argc, argv);
	table = init_simulation(argc, argv);
	creat_philo_thread(table);
	start_simulation(table);
	check_simulation(table);
}
