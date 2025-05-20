/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:42:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/05/20 13:42:07 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include<unistd.h>
# include<stdio.h>
# include<stdlib.h>
# include<stddef.h>
# include<string.h>
# include<sys/time.h>
# include<pthread.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				fork;
	int				count_eat;
	long long		h_last_meal;
	long long		t_since_last_meal;
	long long		h_of_die;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	long long		time_start;
	long long		actual_ms;
	int				philo_nb;
	int				odd_even;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meal;
	int				flag_death;
	int				flag_thread;
	int				count_meal;
	pthread_t		*th;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	t_philo			*philo;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	synchro;
	pthread_mutex_t	death_mutex;
}	t_table;

// Fonctions utilitaires
int			ft_atoi(const char *nptr);
int			ft_usleep(size_t milliseconds);
long long	get_time(void);
long long	get_timestamp(t_table *table);

// Gestion erreurs et mémoire
void		error_msg(char *msg);
void		error_free_msg(char *msg, t_table *table);
void		error_free_msg_nb(char *msg, t_table *table, int i);
void		destroy_all_mutex(t_table *table);
void		free_all(t_table *table);

// Initialisation et vérifications
void		check_args_1(int argc, char **argv);
void		check_args_2(int argc, char **argv);
void		check_odd_even(int nb, t_table *table);
void		alloc_other(t_table *table);
void		init_philo(t_table *table);
void		creat_philo_thread(t_table *table);
void		join_philo_thread(t_table *table);
void		init_forks(t_table *table);
void		init_print(t_table *table);
void		init_other_mutex(t_table *table);
t_table		*init_table(int argc, char **argv);
t_table		*init_simulation(int argc, char **argv);

// Routine philo
int			check_flag_death(t_table *table);
int			check_flag_threads(t_table *table);
int			take_first_fork(t_table *table, t_philo *philo);
int			take_second_fork(t_table *table, t_philo *philo);
long long	check_h_of_die_i(t_table *table, int i);
void		release_fork(t_philo *philo);
void		eating(t_table *table, t_philo *philo);
void		sleeping(t_table *table, t_philo *philo);
void		thinking(t_table *table, t_philo *philo);
void		display_msg(t_table *table, t_philo *philo, char *msg_action);
void		display_die_msg(t_table *table, t_philo *philo, char *msg_action);
void		desynchro(t_table *table, t_philo *philo);
void		*start_rout(void *philo_ptr);
void		routine(t_table *table, t_philo *philo);
void		start_simulation(t_table *table);
void		check_philo_death(t_table *table, t_philo *philo);
void		check_count_meal(t_table *table);
void		check_simulation(t_table *table);

#endif