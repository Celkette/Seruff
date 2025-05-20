/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_begin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:43:23 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 14:43:25 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

void	check_args_1(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		error_msg("Error: Invalid number of arguments");
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '\0')
			error_msg("Error: an argument is empty");
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][0] == '-')
				check_args_2(argc, argv);
			if (argv[i][0] == '+')
				j++;
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
				error_msg("Error: argument must be number");
		}
		i++;
	}
}

void	check_args_2(int argc, char **argv)
{
	if (ft_atoi(argv[1]) < 1)
		error_msg("Error: argument number of philosopher must be > 0");
	if (ft_atoi(argv[2]) <= 0)
		error_msg("Error: argument time of death must be > 0");
	if (ft_atoi(argv[3]) <= 0)
		error_msg("Error: argument time to eat must be > 0");
	if (ft_atoi(argv[4]) <= 0)
		error_msg("Error: argument time to sleep must be > 0");
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		error_msg("Error: argument number of meal must be > 0");
}

void	check_odd_even(int nb, t_table *table)
{
	if (nb % 2 == 0)
		table->odd_even = 2;
	else
		table->odd_even = 1;
}
