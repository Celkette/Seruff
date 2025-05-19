#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

// Déclaration externe de la variable mutex
//extern pthread_mutex_t mutex;
typedef struct s_table t_table;

typedef struct s_philo
{
	int id; // id du philosophe
	int fork;//index de la fourchette
	int 	l_fork; // index de la fourchette de gauche
	int 		r_fork; // index de la fourchette de droite
	long long h_last_meal; // heure du debut du dernier repas
	long long t_since_last_meal;// temps depuis le dernier repas
	long long h_of_die; // heure de mort ==  h_start_meal + time_to_die
	int count_eat;//nombre de repas pris par philo
	t_table *table;// pointeur vers la table - permet au philo individuel d'acceder aux variables de la table
} t_philo;

typedef struct s_table
{
	long long time_start;//debut de la simulation
	long long actual_ms;//ms depuis le debut de la simulation == actual_time % 1000 - time_start
	int 	philo_nb;//nombre de philosophes
	int 	odd_even;//si impaire = 1, si paire = 2
	int 	time_to_die; // temps avant de mourir de faim
	int time_to_eat;//temps pour manger
	int time_to_sleep;//temps pour dormir
	int nb_meal;//nombre de repas defini par les arguments
	int flag_death;//flag de mort == 1 si mort, 0 sinon
	int flag_thread;//flag de creation des threads == 0 puis incrementé
	int count_meal;//nombre de repas pris par tous les philosophes
	pthread_t *th; // tableau de threads
	pthread_mutex_t *fork; // mutex pour les fourchettes
	pthread_mutex_t print; // mutex pour l'affichage
	t_philo *philo; //  pointeur vers le tableau de philosophes
	pthread_mutex_t eat_mutex; // mutex d'incrementation du nombre de repas pris
	pthread_mutex_t synchro; // mutex pour la synchronisation (flag_thread)
	pthread_mutex_t death_mutex;// mutex pour flag_mutex
}   t_table;

int			ft_atoi(const char *nptr);
long long	get_time(void);
long long	get_timestamp(t_table *table);
void		error_msg(char *msg);
void		error_msg_mutex(char *msg, t_table *table);
void		error_free_msg(char *msg, t_table *table);
void		error_free_msg_nb(char *msg, t_table *table, int i);
void		destroy_all_mutex(t_table *table);
void		free_all(t_table *table);
void		check_args_1(int argc, char **argv);
void		check_args_2(int argc, char **argv);
void		check_alone_philo(char **argv);
void		check_odd_even(int nb, t_table *table);
void		alloc_other(t_table *table);
void		init_philo(t_table *table);
void creat_philo_thread(t_table *table);
void creat_monitor_thread(t_table *table);
void join_philo_thread(t_table *table);
void init_forks(t_table *table);
void init_print(t_table *table);
int take_forks(t_table *table, t_philo *philo);
void release_fork(t_table *table, t_philo *philo);
void eating(t_table *table, t_philo *philo);
void sleeping(t_table *table, t_philo *philo);
void thinking(t_table *table, t_philo *philo);
void display_msg(t_table *table, t_philo *philo, char *msg_action);
void *start_rout(void *philo_ptr);
void routine(t_table *table, t_philo *philo);
void start_simulation(t_table *table);
void check_philo_death(t_table *table, t_philo *philo);
void check_count_meal(t_table *table);
void stop_free_simulation(t_table *table);
void check_simulation(t_table *table);
t_table *init_table(int argc, char**argv);
t_table *init_simulation(int argc, char **argv);

#endif