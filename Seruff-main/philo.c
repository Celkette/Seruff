// gcc -Wall -Wextra -Werror philo.c -lpthread

//Visualizer
//https://nafuka11.github.io/philosophers-visualizer/

#include "include/philo.h"
int check_flag_death(t_table *table)
{
	int death;

	pthread_mutex_lock(&table->death_mutex);
	death = table->flag_death;
	pthread_mutex_unlock(&table->death_mutex);
	return (death);
}
//retourne le flag nbr de thread cree
int check_flag_threads(t_table *table)
{
	int threads;

	pthread_mutex_lock(&table->synchro);
	threads = table->flag_thread;
	pthread_mutex_unlock(&table->synchro);
	return (threads);
}

//retourne l'heure de la mort potentielle du philo i
long long int check_h_of_die_i(t_table *table, int i)
{
	long long int h_death;

	pthread_mutex_lock(&table->eat_mutex);
	h_death = table->philo[i].h_of_die;
	pthread_mutex_unlock(&table->eat_mutex);
	return (h_death);
}
//verifier argument
// -nombre
// -si uniquement chiffres
// -verifier si 1 philo -> cas particulier
void error_msg(char *msg)
{
	printf("%s\n", msg);
	perror(NULL);
	exit(EXIT_FAILURE);
}
void error_msg_mutex(char *msg, t_table *table)
{
	(void)table;
	//pthread_mutex_lock(&table->print);
	printf("%s\n", msg);
	//pthread_mutex_unlock(&table->print);
	perror(NULL);
	exit(EXIT_FAILURE);
}

void error_free_msg(char *msg, t_table *table)
{
	//pthread_mutex_lock(&table->print);
	printf("%s\n", msg);
	//pthread_mutex_lock(&table->print);
	perror(NULL);
	if (table && table->th)
        join_philo_thread(table); // Ajoute ceci si des threads ont été créés
	destroy_all_mutex(table);
	free_all(table);
	exit(EXIT_FAILURE);
}
void error_free_msg_nb(char *msg, t_table *table, int i)
{
	//pthread_mutex_lock(&table->print);
	printf("%s pour philo id %d\n", msg, i);
	//pthread_mutex_lock(&table->print);
	perror(NULL);
	if (table && table->th)
        join_philo_thread(table); // Ajoute ceci si des threads ont été créés
	destroy_all_mutex(table);
	free_all(table);
	exit(EXIT_FAILURE);
}
//free fork-thread-philo-table
void destroy_all_mutex(t_table *table)
{
    int i = 0;
    if (!table)
        return;
    if (table->fork)
	{
        while (i < table->philo_nb)
		{
            if (pthread_mutex_destroy(&table->fork[i]))
				error_msg("Error: destroy mutex fork failed\n");
printf("destroy fork mutex %d\n", i);		
			i++;
		}
	}
	if (pthread_mutex_destroy(&table->print))//detruit mutex print
		error_msg("Error: destroy mutex print failed\n");
printf("destroy print mutex\n");
	if (pthread_mutex_destroy(&table->eat_mutex))//detruit mutex print
		error_msg("Error: destroy eat_mutex failed\n");
printf("destroy eat mutex\n");
	if (pthread_mutex_destroy(&table->synchro))//detruit mutex print
		error_msg("Error: destroy synchro_mutex failed\n");
printf("destroy synchro mutex\n");
	if (pthread_mutex_destroy(&table->death_mutex))//detruit mutex print
		error_msg("Error: destroy death_mutex failed\n");
printf("destroy death mutex\n");
}
void free_all(t_table *table)
{
	if (table)
	{
		if (table->fork)// Libère le tableau de mutex fourchettes
		{    
			free(table->fork);
printf("table->fork freed\n");
		}
		if (table->th)// Libère le tableau de mutex thread 
		{ 
			free(table->th);
printf("table->th freed\n");
		}
		if (table->philo)// Libère le tableau de structure t_philo
		{
			free(table->philo);
printf("table->philo freed\n");
		}
		free(table);// Libère la structure t_table
printf("table freed\n");
	}
}
int	ft_atoi(const char *nptr)
{
	int	sign;
	int	result;
	int	i;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		result = result * 10 +(nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
/***************************************
*            Compte du temps           *
****************************************/
// Version améliorée de la fonction sleep 
int 	ft_usleep( size_t milliseconds)
 { 
	size_t 	start; 

	start = get_time(); 
	while ((get_time() - start) < milliseconds) 
		usleep( 500 ); 
	return ( 0 ); 
}
//Heure actuelle = recupere temps en millisecondes depuis l'epoque
long long int get_time(void)
{
	struct timeval time;

	if (gettimeofday(&time, NULL) != 0)//donne l'heure actuelle en secondes et microsecondes
	{
		perror("gettimeofday failed");
		return (-1);
	}
	//tv_sec == nbr de secondes écoulées depuis l'époque
	//tv_usec == nbr de microsecondes écoulées depuis le début de la seconde actuelle
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
//Horodatage = calcul du temps passe depuis le debut de la simulation en millisecondes
long long int get_timestamp(t_table *table)
{
	long long timestamp;

	timestamp = get_time() - table->time_start;
	return (timestamp);
}
/***************************************
*           Differents check           *
****************************************/
//verifie nb arg, si chiffres, si positif
void check_args_1(int argc, char **argv)
{
	int i;
	int j;

	if (argc < 5 || argc > 6)//nb d'arguments
		error_msg("Error: Invalid number of arguments");
	i = 1;
	while (i < argc)//verifie si les arguments sont des chiffres
	{
		if (argv[i][0] == '\0')//si arg vide ("")
			error_msg("Error: an argument is empty");
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][0] == '-')//si arg negatif
				check_args_2(argc, argv);
			if (argv[i][0] == '+')
				j++;
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else//si autre que des chiffres
				error_msg("Error: argument must be number");
		}
		i++;
	}
}
//verifie si les arguments sont > 0
void check_args_2(int argc, char **argv)
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
//Verifie i un seul philo, et ferme tout et fini programme
void check_alone_philo(char **argv)
{
	if (ft_atoi(argv[1]) == 1)
	{
		
printf("only one philosopher\n");
	}
}
//verifie le nombre de philo (pair ou impair) =>renvoie 1 si impair(odd), 2 si pair(even)
void check_odd_even(int nb, t_table *table)
{
	if (nb % 2 == 0)
		table->odd_even = 2;
	else
		table->odd_even = 1;
}
/***************************************
*          Allocation memoire          *
* struct philo + thread + fork *
****************************************/
//allocation memoire pour structure philo et mutex thread + fork
void alloc_other(t_table *table)
{
	table->th = malloc(sizeof(pthread_t) * table->philo_nb);//th = thread
	if (!table->th)//init_thread(table);
		error_free_msg("Error: malloc pthread_t thread failed", table);
	table->fork  = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	if (!table->fork )//init_forks(table);
		error_free_msg("Error: malloc pthread_mutex_t fork failed", table);
	table->philo = malloc(sizeof(t_philo) * (table->philo_nb));
	if (!table->philo)//table->philo est un tabl de struct t_philo, et non un tabl de pointeurs vers des struct t_philo.
		error_free_msg("Error: malloc t_philo philo failed", table);
}
/***************************************
*        Initialisation variable       *
****************************************/
//initialise la structure table(init variable de la table)
t_table *init_table(int argc, char**argv)
{
	t_table *table;
	
	table = malloc(sizeof (t_table));
	if (!table)
		error_msg("Error: malloc failed");
	table->philo_nb = ft_atoi(argv[1]);
	check_odd_even(table->philo_nb, table);//init table->odd_even
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->nb_meal = -1;
	if (argc == 6)
		table->nb_meal = ft_atoi(argv[5]);
	table->count_meal = 0;
	table->time_start = 0;
	table->actual_ms = 0;
	table->flag_death = 0;
	table->th = NULL;
	table->fork = NULL;
	table->philo = NULL;
	return (table);
}
//initialise la structure philo(init variable des philosophes)
void init_philo(t_table *table)
{
	int i;

	i = 0;
	while (i < table->philo_nb)
	{
		table->philo[i].id = i + 1; // Initialisation de l'ID du philosophe
		table->philo[i].l_fork = table->fork[i];
		table->philo[i].r_fork = table->fork[(i + 1) % table->philo_nb];
		table->philo[i].h_last_meal = 0;
		table->philo[i].t_since_last_meal = 0;
		table->philo[i].h_of_die = 2000000000000;// Initialisation de l'heure de mort
		table->philo[i].count_eat = 0;
		table->philo[i].table = table; // Associer la table au philosophe(pointeur vers la table)
		
		i++;
	}
}
/***************************************
*        creation thread et mutex      *
****************************************/
//cree les threads des philosophes
void creat_philo_thread(t_table *table)
{
	int i;

	i = 0;
	table->flag_thread = 0;
	while (i < table->philo_nb)
	{					  //&table->th[i] = pointeur vers la structure t_philo du i-ème philosophe
		if (pthread_create(&table->th[i], NULL, start_rout, &table->philo[i]))//thread[i] executera start_rout(table->philo[i])
			{
				error_free_msg_nb("Error: create_thread philo failed", table, i);
			}
		pthread_mutex_lock(&table->synchro);
		table->flag_thread++;
		pthread_mutex_unlock(&table->synchro);
		i++;
	}
}

//joint les threads du monitor(surveillance de la simulation) et des philosopher
//demande au programme d'attendre la fin des threads pour se finir
void join_philo_thread(t_table *table)
{
	int i;

	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_join(table->th[i], NULL))//attend la fin du thread(renvoie 0 si succes)
			error_free_msg_nb("Error: join_thread philo failed", table, i);
//printf("Thread %d joined\n", i);
		i++;
	}
//printf("%d threads philo joined\n", i);
}
//Creer les mutex des fourchetinit_simulation(argc, argv);tes
void init_forks(t_table *table)
{
   int i;

   i = 0;
   while (i < table->philo_nb)
   {
		if (pthread_mutex_init(&table->fork[i], NULL))
			error_free_msg_nb("Error: init_fork mutex failed", table, i);
		i++;
   }
}
//Creer le mutex du print
void init_print(t_table *table)
{
	if (pthread_mutex_init(&table->print, NULL))
		error_free_msg("Error: init_print mutex failed", table);
}
void init_other_mutex(t_table *table)
{
	if (pthread_mutex_init(&table->eat_mutex, NULL))//init eat_mutex
		error_free_msg("Error: init_eat_mutex failed", table);//init synchro_mutex
	if (pthread_mutex_init(&table->synchro, NULL))
		error_free_msg("Error: init_flag_thread_mutex failed", table);
	if (pthread_mutex_init(&table->death_mutex, NULL))//init death_mutex
		error_free_msg("Error: init_death_mutex failed", table);
}
/***************************************
*        Initialisation simulation     *
****************************************/
//alloue et initialise la t_table
//alloue et initialise les thread th(tableau de philo) et struct t_philo , et les mutex fork et print
//initialise le thread monitor (thread de simulation)
t_table *init_simulation(int argc, char **argv)
{
	t_table *table;
	table = init_table(argc, argv);//initialise la structure table
	alloc_other(table);//allocation memoire pour structure philo et mutex thread + fork
	init_philo(table);//initialise la structure philo
	init_forks(table);//Creer les mutex des fourchettes
	init_print(table);//Creer le mutex du print
	init_other_mutex(table);//Creer le mutex du nombre de repas pris
	return (table);
}
/***************************************
*         Action de la simulation      *
****************************************/
//philo tente de prendre les fourchettes
//imprime "has taken r_fork" ou " l_fork" selon la fourchette prise
/*void take_forks(t_table *table, t_philo *philo)
{
	int first;
	int second;
	
		if (philo->r_fork > philo->l_fork )//verrouille toujours la fourchette index le plus grand en 1er
		{
			first = philo->r_fork;
			second = philo->l_fork;
			pthread_mutex_lock(&table->fork[first]);//verrouille la fourchette dt;
			display_msg(table, philo, "has taken r_fork");
			pthread_mutex_lock(&table->fork[second]);
			display_msg(table, philo, "has taken l_fork");
		}
		else
		{
			first = philo->l_fork;
			second = philo->r_fork;
			pthread_mutex_lock(&table->fork[first]);//verrouille la fourchette dt;
			display_msg(table, philo, "has taken l_fork");
			pthread_mutex_lock(&table->fork[second]);
			display_msg(table, philo, "has taken r_fork");
		}
}*/
//imprime "has taken a fork"
int take_forks(t_table *table, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->r_fork);
		display_msg(table, philo, "has taken a fork");
		pthread_mutex_lock(&philo->l_fork);
		display_msg(table, philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->l_fork);
		display_msg(table, philo, "has taken a fork");
		pthread_mutex_lock(&philo->r_fork);
		display_msg(table, philo, "has taken a fork");
	}
	return (2);//as pris 2 fourchettes
}
void release_fork(t_table *table, t_philo *philo)
{
	(void)table;
	pthread_mutex_unlock(&philo->r_fork);//deverrouille la fourchette dt;
	//display_msg(table, philo, "unlock r_fork");  
	pthread_mutex_unlock(&philo->l_fork);//deverrouille la fourchette gch;
	//display_msg(table, philo, "unlock l_fork");
}
//philo mange
void eating(t_table *table, t_philo *philo)//argument en millisecondes
{
	display_msg(table, philo, "is eating");
	pthread_mutex_lock(&table->eat_mutex);//verrouille le nombre total de repas pris
	philo->h_last_meal = get_time();//stocke l'heure du dernier repas
	philo->h_of_die = philo->h_last_meal + table->time_to_die;//stocke l'heure de mort
	philo->count_eat++;//incremente le nombre de repas pris
	table->count_meal++;//incremente le nombre de repas pris par tous les philosophes
	pthread_mutex_unlock(&table->eat_mutex);//deverrouille le nombre total de repas pris
	//usleep(table->time_to_eat * 1000);//en microsecondes
	ft_usleep(table->time_to_eat);
}
//philo dort
void sleeping(t_table *table, t_philo *philo)
{
	display_msg(table, philo, "is sleeping");
	//usleep(table->time_to_sleep * 1000);//en microsecondes
	ft_usleep(table->time_to_sleep );
}
//philo pense
void thinking(t_table *table, t_philo *philo)
{
	long long int t_before_die;

	display_msg(table, philo, " is thinking");
	pthread_mutex_lock(&table->eat_mutex);
	t_before_die = philo->h_of_die - get_time();
	pthread_mutex_unlock(&table->eat_mutex);
//A SUPPIRIMER verif de bon timing de la mort
	if (t_before_die <= 0)
	{
		display_msg(table, philo, "devrait deja etre mort");
		return ;
	}
//
	//usleep(t_before_die / 2);//en microsecondes
	ft_usleep(t_before_die / 2);
}
//affiche l'horodatage + id du philo + action
void display_msg(t_table *table, t_philo *philo, char *msg_action)
{
		pthread_mutex_lock(&table->print);//verrouille le print;
		printf("%lld %d %s\n", get_timestamp(table), philo->id, msg_action);//print l'heure + id du philo + action
		pthread_mutex_unlock(&table->print);//deverrouille le print;
}
/***************************************
*            debut et routine          *
****************************************/
//initialisation time_start - h_last_meal - h_of_die
//mis direct dans start_simulation
void start_point(t_table *table)
{
	int i;
	long long int start;

	start = get_time();//HEURE DE DEMARRAGE DE LA SIMULATION
	i = 0;
	pthread_mutex_lock(&table->eat_mutex);
	table->time_start = start;
	pthread_mutex_unlock(&table->eat_mutex);
	while (i < table->philo_nb)
	{
		pthread_mutex_lock(&table->eat_mutex);
		table->philo[i].h_last_meal = start;//fait demarer tous les philo en meme temps
		table->philo[i].h_of_die = start + table->time_to_die;
		pthread_mutex_unlock(&table->eat_mutex);
		i++;
	}
}
//desynchronisation depart selon nbr philo et leur id
void desynchro(t_table *table, t_philo *philo)
{
	if (table->odd_even == 2)//si nb philo paire
	{
		if (philo->id % 2 == 0)//si philo id impaire
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2);
			//usleep((table->time_to_eat / 2);
		}
	}
	if (table->odd_even == 1)//si nb philo impaire
	{
		if (philo->id % 2 != 0 && philo->id != 1)//si philo id impaire et != 1
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2 * 3);
			//usleep(table->time_to_eat/ 2 * 3);
		}
		if (philo->id % 2 == 0)
		{
			display_msg(table, philo, "is thinking");
			ft_usleep(table->time_to_eat / 2);
			//usleep(table->time_to_eat/ 2);
		}
	}	
}
//routine 
void *start_rout(void *philo_ptr)
{
	t_table *table;
	t_philo *philo;

	philo = (t_philo *)philo_ptr;//recupere un pointeur vers structure t_philo qui a été
//passée au thread start lors de sa création avec pthread_create dans creat_philo_thread(table).
	table = philo->table;//recupere un pointeur vers la structure t_table via la structure philo
	while (1)//attente de l'initialisation de table->time_start
	{
		pthread_mutex_lock(&table->eat_mutex);
		if (table->time_start != 0)//time_start est init à 0 donc si !=0, simu peut debuter
		{
			pthread_mutex_unlock(&table->eat_mutex);
			break;
		}
		pthread_mutex_unlock(&table->eat_mutex);
		ft_usleep(1);
	}
	desynchro(table, philo);//desynchronisation depart selon nbr philo et leur id
	routine(table, philo);
	return (NULL);// Ajout d'un retour pour satisfaire la signature de la fonction
}

void routine(t_table *table, t_philo *philo)
{
	while (check_flag_death(table) == 0)//routine tant que pas de mort
	{
		if (take_forks(table, philo) == 1)//si un seul philo
		{
			ft_usleep(table->time_to_die);
        //		pthread_mutex_unlock(&table->fork[philo->r_fork]);
        		break;
		}
		else if (check_flag_death(table) != 0)
		{
			release_fork(table, philo);
        	break;
		}
		eating(table, philo);
		release_fork(table, philo);
		if (check_flag_death(table) != 0)
            break;
		sleeping(table, philo);
		if (check_flag_death(table) != 0)
            break;
		thinking(table, philo);
	}
}
//start (depart differé pour philosophes 1, paire et impaires)
//appellé par pthread_create de creat_philo_thread
void start_simulation(t_table *table)
{
	int i;
	long long int start;

	while (check_flag_threads(table) != table->philo_nb)//attend que tous les threads soient crees
	{
		//usleep(1);
		ft_usleep(1);
	}
	//start_point(table);//initialisation de time_start - h_last_meal - h_of_die
	start = get_time();//HEURE DE DEMARRAGE DE LA SIMULATION
	i = 0;
	pthread_mutex_lock(&table->eat_mutex);
	table->time_start = start;
	pthread_mutex_unlock(&table->eat_mutex);
	while (i < table->philo_nb)
	{
		pthread_mutex_lock(&table->eat_mutex);
		table->philo[i].h_last_meal = start;//fait demarer tous les philo en meme temps
		table->philo[i].h_of_die = start + table->time_to_die;
		pthread_mutex_unlock(&table->eat_mutex);
		i++;
	}
}
//*******************************************************************

//*******************************************************************


/***************************************
*        check mort et nb repas      *
****************************************/
//verifie si tous les philosophes sont morts
void check_philo_death(t_table *table, t_philo *philo)
{
	int i;

	i =0;
	while (i < table->philo_nb)
	{
		if (get_time() >= check_h_of_die_i(table, i))
		{
			pthread_mutex_lock(&table->death_mutex);
			table->flag_death = 1;
			pthread_mutex_unlock(&table->death_mutex);
			display_msg(table, &philo[i], "died\n");
			break;
		}
		i++;
	}
}
//verifie si tous les philosophes ont mange le nb de repas defini
void check_count_meal(t_table *table)
{
	if (table->nb_meal != -1)// Vérifie si un nombre de repas max a été défini
	{
		pthread_mutex_lock(&table->eat_mutex);
		if (table->count_meal == (table->nb_meal * table->philo_nb))
		{
			pthread_mutex_unlock(&table->eat_mutex);
			pthread_mutex_lock(&table->death_mutex);
			table->flag_death = 2;
			pthread_mutex_unlock(&table->death_mutex);
		}
	}
}
//Arrête tout si un philo est mort ou si le nb de repas est atteint
//destroy mutex fork et print et liberation memoire
void stop_free_simulation(t_table *table)
{
	destroy_all_mutex(table);
	free_all(table);//free fourchette-tableau th-tableau struct philo-struct table
}

void check_simulation(t_table *table)
{
	ft_usleep(table->time_to_die -2);//pause avant de demarrer le check VOIR POUR ADAPTATION
	while (check_flag_death(table) == 0)//tant que pas de mort
	{
		//usleep(1);
		ft_usleep(1);//ajouter 1ms delai pour eviter de trop polluer le CPU (entre 1 et 5ms)
		check_philo_death(table, table->philo);
		check_count_meal(table);
	}
	if (check_flag_death(table) != 0)//si mort d'un philo ou si nb repas atteint
	{
//pthread_mutex_lock(&table->print);
//printf("+++ appel join_philo_thread\n");
//pthread_mutex_unlock(&table->print);
		join_philo_thread(table);
//pthread_mutex_lock(&table->print);
//printf("+++ appel de stop_free_simulation\n");
//pthread_mutex_unlock(&table->print);
	stop_free_simulation(table);
	}
}


int main(int argc, char **argv)
{
	t_table *table;// Déclaration du pointeur 'table'
		
	check_args_1(argc, argv);
	//check_alone_philo(argv);
	table = init_simulation(argc, argv);
	creat_philo_thread(table);//Cree les threads des philosopher
	start_simulation(table);
	check_simulation(table);	
}


