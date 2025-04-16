/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:57:55 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:57:57 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	i;

	i = 0;
	dest = (char *)malloc(ft_strlen(src) + 1);
	if (dest == NULL)
		return (NULL);
	while (src[i] != 0)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
	return (dest);
}
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*news;

	i = 0;
	news = NULL;
	if (!s)
		return (ft_strdup(""));
	if (start >= (unsigned int)ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	news = malloc((len + 1) * sizeof(char));
	if (!news)
		return (NULL);
	while (i < len && (s[start + i] != '\0'))
	{
			news[i] = s[start + i];
			i++;
	}
	news[i] = '\0';
	return (news);
}
static int	ft_count_word(char const *str, char c)
{
	int	n_word;
	int	in_word;
	int	i;

	n_word = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && !in_word)
		{
			in_word = 1;
			n_word++;
		}
		else if (str[i] == c)
		{
			in_word = 0;
		}
		i++;
	}
	return (n_word);
}
static void	*ft_free_all(char **dest, int index)
{
	while (index > 0)
	{
		free(dest[index - 1]);
		index--;
	}
	free(dest);
	return (NULL);
}
static char	*ft_find_start_end_write(char const *str, char c, size_t *i)
{
	size_t	start;

	while (str[*i] == c)
		(*i)++;
	start = (*i);
	while (str[*i] != '\0' && str[*i] != c)
		(*i)++;
	return (ft_substr(str, start, (*i - start)));
}
char	**ft_split(char const *s, char c)
{
	int		n_word;
	int		index;
	char	**dest;
	size_t	i;

	n_word = ft_count_word(s, c);
	index = 0;
	i = 0;
	dest = (char **)malloc(sizeof(char *) * (n_word + 1));
	if (!dest)
		return (NULL);
	while (index < n_word)
	{
		dest[index] = ft_find_start_end_write(s, c, &i);
		if (!dest[index])
		{
			ft_free_all(dest, index);
			return (NULL);
		}
		index++;
	}
	dest[index] = NULL;
	return (dest);
}
int	ft_isspace(int i)// dans utils_libft.c
{
	if (i == 32 || (i >= 9 && i <= 13))
		return (1);
	return (0);
}
int	ft_isdigit(int i)// dans utils_libft.c
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}
long	ft_atol(const char *str)// dans utils_libft.c
{
	int	sign;
	long	result;
	int	i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0' && ft_isdigit(str[i]) == 1)
	{	
		result = result * 10 +(str[i] - '0');
		i++;
	}
	return (result * sign);
}

//verifier si dans libft
void	ft_free_tab(char **tab)
{
	int	i;

	if (tab == NULL)
		return ;
	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

//------------VERIFICATION ARGUMENTS-------------------

 //verifie qu'il s'agit bien de chiffre (espace NON autorisé)
int	ft_args_isdigit(char *str)
{
    int i;
    int in_digit;
    
    i = 0;
    in_digit = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '-' || str[i] == '+')
        {
            if (in_digit == 1 || str[i + 1] == '\0' || !ft_isdigit(str[i + 1]))
                return (1);
            in_digit = 1;
        }
        else if (ft_isdigit(str[i]))
            in_digit = 1;
        else
            return (1);
        i++ ;
    }
    return (0);
}

int ft_is_space_inside(const char *str)
{
    int i = 0;
    int digit_seen = 0;

    while (str && str[i] != '\0' && ft_isspace(str[i]))
        i++;
    while (str && str[i] != '\0')
    {
        if (ft_isdigit(str[i]))
            digit_seen = 1;
        else if (ft_isspace(str[i]) && digit_seen)
        {
            int j = i + 1;
            while (str[j] != '\0' && ft_isspace(str[j]))
                j++;
            if (ft_isdigit(str[j]))
                return (1);
        }
        i++;
    }
    return (0);
}

int check_max_min(char *n)
{
    if (ft_atol(n) > INT_MAX || ft_atol(n) < INT_MIN)
        return (1);
    return (0);
}
//verifie les doubles si argv[1] = string
int check_double_if_string_arg(char **array)
{
	int i;
	int j;

	i = 0;
	while (array[i] != NULL)
	{
		j = i + 1;
		while (array[j] != NULL)
		{
			if (ft_atol(array[i]) - ft_atol(array[j]) == 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

//check argument si argument si ./a.out arg arg arg ...
int check_nb(int argc, char **argv)
{
    int i;
	int j;

    i = 1;
    while (i < argc)
    {
        if (ft_args_isdigit(argv[i]) == 1 || check_max_min(argv[i]))
            return (1);
        i++;
    }
	i = 1;
	while (i < argc)
	{
		j = i + 1;
		while (j < argc)
		{
			if ((ft_atol(argv[i]) - ft_atol(argv[j]) == 0))
				return (1);
			j++;
		}
		i++;
	}
    return (0);
}

//appel toutes les autres = MAIN
int check_argument(int argc, char **argv)//appel toutes les autres = MAIN
{
    if (argc == 2 && ft_is_space_inside(argv[1]) == 1)
    {
            write(2, "Error\n", 6);
            exit (EXIT_FAILURE);
    }
    else if (argc >= 2)
    {
        if (check_nb(argc, argv) == 1)
        {
            write(2, "Error\n", 6);
            exit (EXIT_FAILURE);
        }
    }
    else
        exit (EXIT_FAILURE);
    return (0);
}

//------------CREATION LISTE A INDEXÉE-------------
//cree une liste
t_list *ft_creat_list(void)
{
    t_list   *new_list;
    
    new_list = malloc(sizeof(t_list));
    if (new_list == NULL)
    {
        perror("Erreur d'allocation mémoire pour creat_list");
        exit(EXIT_FAILURE);
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return (new_list);
}
//cree un element
t_element   *ft_creat_element()
{
    t_element   *new;

    new = (t_element *)malloc(sizeof(t_element));
    if (new == NULL)
    {
        perror("Erreur d'allocation mémoire pour creat_element");
        return (NULL);
    }
    new->next = NULL;
    new->prev = NULL;
    new->data = 0;
    new->index = 0;
    return (new);
}
//cree une copy de l'element current_a sur la queue de list_c
t_element *ft_creat_and_init_copy_tail(t_list *list_c, t_element *current_a)//pour main swap.c+push.c
{
    t_element *new_tail;

    new_tail = ft_creat_element();
    if (list_c == NULL || new_tail == NULL)
        return (NULL);
    if (list_c->head == NULL)
    {
        list_c->head = new_tail;
        list_c->tail = new_tail;
        list_c->tail->next = list_c->head;
        list_c->head->prev = list_c->tail;
    }
    else
    {
        new_tail->prev = list_c->tail;
        list_c->tail->next = new_tail;
        list_c->tail = new_tail;
        list_c->head->prev = new_tail;
        new_tail->next = list_c->head;
    }
    new_tail->data = current_a->data;
    new_tail->index = current_a->index;
    return (new_tail);
}
//initialise l'element add sur la queue de list
void ft_init_element_tail(t_list *list, t_element *add)//pour main swap.c+push.c
{
    if (list == NULL || add == NULL)
        return;
    if (list->head == NULL)
    {
        list->head = add;
        list->tail = add;
        add->next = add;
    }
    else
    {
        add->prev = list->tail;
        list->tail->next = add;
        list->tail = add;
        list->head->prev = add;
        add->next = list->head;
    }

}
//index les element d'une liste
void ft_indexage_list(t_list *list)
{
    int i;

    i = 1;
    while (i <= list->size)
    {
        list->head->index = i;
        list->head = list->head->next;
        i++;
    }
}
//cree une copie de list_a
t_list *ft_copy_list(t_list *list_a)
{
    t_list *list_c;
    t_element *current_a;
    int i;

    if (!list_a || list_a->head == NULL)
        return (NULL);
    list_c = ft_creat_list();
    current_a = list_a->head;
    i = 0;
    while (i < list_a->size)
    {
        ft_creat_and_init_copy_tail(list_c, current_a);
        current_a = current_a->next;
        list_c->size++;
        i++;
    }
    return (list_c);
}
//echange les valeur de 2 elements
void ft_swap_data(t_element *a, t_element *b)
{
    long temp = a->data;
    a->data = b->data;
    b->data = temp;
}
//transfere l'indexage ordonné aux element desordonné de list_a
void ft_copy_index(t_list *list_c, t_list *list_a)
{
    t_element *current_a;
    t_element *current_c;
    int i_a;
    int i_c;

    current_a = list_a->head;
    i_a = 0;
    while (i_a < list_a->size)
    {
        current_c = list_c->head;
        i_c = 0;
        while (i_c < list_c->size)
        {
            if (current_a->data == current_c->data)
            {
                current_a->index = current_c->index;
                break;
            }
            current_c = current_c->next;
            i_c++;
        }
        current_a = current_a->next;
        i_a++;
    }
}
//free une liste
void ft_free_list(t_list *list)
{
    t_element *current;
    t_element *next;

    if (list == NULL)
        return;
    if (list->head == NULL)
        return (free(list));
    current = list->head;
    if (current->next == list->head)
    {
        free(current);
        free(list);
        return;
    }
    while (current->next != list->head)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(current);
    free(list);
}
//trie une liste_c dans l'ordre croissant
void   ft_index_list(t_list *list)
{
    t_list *list_c;
    t_element *current;
    
    list_c =  ft_copy_list(list);
    if (list == NULL || (list)->head == NULL || list->size < 1)
    {
        ft_free_list(list_c);
        exit (1);
    }
        current = list_c->head;
    while (current->next != list_c->head)
    {
        if (current->data  > current->next->data )
        {
            ft_swap_data(current, current->next);
            current = list_c->head;
        }
        else
            current = current->next;
    }
    ft_indexage_list(list_c);
    ft_copy_index(list_c, list);
    ft_free_list(list_c);
}

//cree une liste a partir des arguments
t_list *creat_list_a_from_args(int argc, char **argv)
{
    t_list *list_a;
    t_element *new_elem;
    long data;
    int i;

    i = 1;
    list_a = ft_creat_list();
    while (i < argc)
    {
        data = ft_atol(argv[i]);
        new_elem = ft_creat_element();
        new_elem->data = data;
        ft_init_element_tail(list_a, new_elem);
        list_a->size++;
        i++;
    }
    return (list_a);
}
//cree une liste a partir des argument et l'indexe(index dans l'ordre des valeur)
t_list *creat_final_list_a(int argc, char **argv)
{
    t_list *list_a;
    //if (argc == 2)
     //   list_a = creat_list_a_from_string(argv);
    /*else*/ if (argc >= 2)
        list_a = creat_list_a_from_args(argc, argv);
    ft_index_list(list_a);

    return (list_a);
}

//----------------ROTATE--------------------
void ps_rotate(t_list *list)
{
    t_element *first;
    t_element *temp;

    if (!list || !list->head)
        return;
    first = list->head;
    if (list->head->next == list->head)
        return;
    if (list->head->next == list->tail)
    {
        temp = list->head;
        list->head = list->tail;
        list->tail = temp;
    }
    else
    {
        list->head = first->next;
        list->tail = first;
    }
    return ;
}

void rotate_a(t_list *list_a)
{
    ps_rotate(list_a);
    write(1, "ra\n", 3);
}

void rotate_b(t_list *list_b)
{
    ps_rotate(list_b);
    write(1, "rb\n", 3);
}

void rotate_a_and_b(t_list *list_a, t_list *list_b)
{
    ps_rotate(list_a);
    ps_rotate(list_b);
    write(1, "rr\n", 3);
}

//----------------REVERSE ROTATE----------------------
void rev_rotate(t_list *list)
{
    t_element *first;
    t_element *temp;

    if (!list || !list->head)
        return;
    first = list->head;
    if (list->head->next == list->head)
        return;
    if (list->head->next == list->tail)
    {
        temp = list->head;
        list->head = list->tail;
        list->tail = temp;
    }
    else
    {
        list->head = first->prev;
        list->tail = first->prev->prev;
    }
    return ;
}

void rev_rotate_a(t_list *list_a)
{
    rev_rotate(list_a);
    write(1, "rra\n", 4);
}

void rev_rotate_b(t_list *list_b)
{
    rev_rotate(list_b);
    write(1, "rrb\n", 4);
}

void rev_rotate_a_and_b(t_list *list_a, t_list *list_b)
{
    rev_rotate(list_a);
    rev_rotate(list_b);
    write(1, "rrr\n", 4);
}

//----------------SWAP---------------------
void ps_swap(t_list *list)
{
    t_element *first;
    t_element *second;

    if (!list || list->head == NULL || list->head->next == list->head)
        return ;
    first = list->head;
    second = first->next;
    if (list->head->next == list->tail)
        list->head = second;
    else
    {
        second->next->prev = first;
        first->next = second->next;
        first->prev = second;
        second->next = first;
        second->prev = list->tail;
        list->head = second;
        list->tail->next = list->head;
        list->head->prev = list->tail;
    }
    return ;
}

void swap_sa(t_list *list_a)
{
    ps_swap(list_a);
    write(1, "sa\n", 3);
	return ;
}

void swap_sb(t_list *list_b)
{
    ps_swap(list_b);
    write(1, "sb\n", 3);
	return ;
}

void swap_ss(t_list *list_a, t_list *list_b)
{
    ps_swap(list_a);
    ps_swap(list_b);
    write(1, "ss\n", 3);
	return ;
}

//----------------PUSH---------------------
void ft_init_head_list(t_list *list, t_element *add)//fait pointer add sur list->head
{
    if (list == NULL|| add == NULL)
        return;
    if (list->head == NULL)
    {
        list->head = add;
        list->tail = add;
        add->next = add;
        add->prev = add;
    }
    else
    {
        list->head->prev = add;
        add->next = list->head;
        list->tail->next = add;
        add->prev = list->tail;
        list->head = add;
    }
    list->size += 1;
}

//depointe la tete de la liste (!pas de free de head, donc necessite de le faire pointer ailleur)
void ft_point_out_head_list(t_list *list)//depointe list->head de list
{
    t_element *first;

    if (!list || !list->head)
		return ;
    first = list->head;
    if (list->head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
    list->head = first->next;
    list->tail->next = list->head;
    list->head->prev = list->tail;
    }
    list->size -= 1;
}

void ps_push(t_list *list_a, t_list *list_b)
{
    t_element *new_head_b;

    if (!list_a || !list_a->head)
        return ;
    new_head_b = list_a->head;
    ft_point_out_head_list(list_a);
    ft_init_head_list(list_b, new_head_b);
}
// pa = push b vers a
void push_b_to_a(t_list *list_a, t_list *list_b)
{
    ps_push(list_b, list_a);
    write(1, "pa\n", 3);
}
// pb = push a vers b
void push_a_to_b(t_list *list_a, t_list *list_b)
{
    ps_push(list_a, list_b);
    write(1, "pb\n", 3);
}

//------------UTILS ALGORITHME---------------------
//vérifie si les éléments de cette liste sont triés par ordre croissant.

int	ft_check_sort_data(t_list *list)
{
	t_element	*head;

	head = list->head;
	while (head->next != list->head)
	{
		if (head->data < head->next->data)
			head = head->next;
		else
			return (1);
	}
	return (0);
}
int	ft_check_sort_index(t_list *list)
{
	t_element	*head;

	head = list->head;
	while (head->next != list->head)
	{
		if (head->index < head->next->index)
			head = head->next;
		else
			return (1);
	}
	return (0);
}

//Compare l'index du 1er et du 2nd (de head avec head->next)
//Recupere head index
int	get_head_i(t_list *list)
{
    t_element	*head;

    head = list->head;
    return (head->index);
}
//Recupere tail index
int	get_tail_i(t_list *list)
{
    t_element	*tail;

    tail = list->tail;
    return (tail->index);
}
//Amene element->index = i en tete de liste
//Trouver le plus petit index dans la liste
int	ft_find_smallest_index(t_list *list)
{
    t_element	*start;
    t_element	*current;
    int		index;
    
    if (!list || !list->head)
        return (0);
    start = list->head;
    index = list->head->index;
    current = list->head->next;
    while (current != start)
    {
        if (index > current->index)
            index = current->index;
        current = current->next;
    }
    return (index);
}
//Trouver le plus grand index dans la liste
int	ft_find_biggest_index(t_list *list)
{
    t_element	*start;
    t_element	*current;
    int		index;
    
    if (!list || !list->head)
        return (0);
    start = list->head;
    index = list->head->index;
    current = list->head->next;
    while (current != start)
    {
        if (index < current->index)
            index = current->index;
        current = current->next;
    }
    return (index);
}

//Compter chemin en descendant pour atteindre index target_i (return count_ahead)
int count_path_ahead(t_list *list, int target_i)
{
    t_element *current;
    int count_ahead;

    count_ahead = 0;
    if (!list || !list->head)
        return 0;
    current = list->head;
    while (current && current->index != target_i)
    {
        current = current->next;
        count_ahead++;
    }
    return (count_ahead);
}
//Compter chemin en remontant pour atteindre index target_i (return -count_back)
int count_path_back(t_list *list, int target_i)
{
    t_element *current;
    int count_back;

    count_back = 0;
    current = list->head;
    while (current && current->index != target_i)
    {
        current = current->prev;
        count_back++;
    }
    return (count_back);
}
//retourne le count le plus petit(positif pour ahead, negatif pour back 
int find_shortest_path(t_list *list, int target_i)
{
    int count_ahead;
    int count_back;

    if (!list || !list->head)
        return 0;
    count_ahead = count_path_ahead(list, target_i);
    count_back = count_path_back(list, target_i);
    if (count_ahead <= count_back)
        return (count_ahead);
    else
        return (-count_back);
} 


//Compter chemin en descendant pour atteindre 1 element <= limit (return count_ahead)
int count_path_ahead_until(t_list *list, int limit)
{
    t_element *current;
    int count_ahead;

    count_ahead = 0;
    current = list->head;

    if (list->size == 1 && current->index <= limit)
        return (0);
    while (current->next != list->head)
    {
        if (current->index > limit)
        {
            current = current->next;
            count_ahead++;
        }
        else
            break;
    }
    return (count_ahead);
}
//Compter chemin en remontant pour atteindre 1 element <= limit (return count_ahead)
int count_path_back_until(t_list *list, int limit)
{
    t_element *current;
    int count_back;

    count_back = 0;
    current = list->head;

    if (list->size == 1 && current->index <= limit)
        return (0);
    while (current->prev != list->head)
    {
        if (current->index > limit)
        {
            current = current->prev;
            count_back++;
        }
        else
            break;
    }
    return (count_back);
}
//Amene list_a->head sur 1 element <= limit (via 1er element rencontre par le bas et par le haut)
void move_shortest_path_until(t_list *list_a, int limit)
{
    int count_ahead;
    int count_back;

    if (!list_a || !list_a->head)
        return ;
    count_ahead = count_path_ahead_until(list_a, limit);
    count_back = count_path_back_until(list_a, limit);
    if (count_ahead <= count_back)
        while (count_ahead-- > 0)
            rotate_a(list_a);
    else
        while (count_back-- > 0)
            rev_rotate_a(list_a);
}

//cherche si 1 element de list est entre 0 et <= limit (si trouve renvoie 1)
int	ft_search_if_limit_in_list(t_list *list, int limit)
{
	t_element	*current;

    if (!list || !list->head)
        return (0);
    current = list->head;
	while (current->next != list->head)
	{
		if (current->index <= limit)
			return (1);
		current = current->next;
	}
    if (list->size == 1 && current->index <= limit)
            return (1);
	return (0);
}
        
//Amener list'c'->head  sur l'element à l'index target_i 
void	bring_to_head(t_list *list, int target_i, char c)//V1
{
    int distance ;

    if (!list || !list->head || list->size <= 1) // Gérer le cas d'une liste vide
        return ;
    distance = find_shortest_path(list, target_i);
    if (distance > 0)
        while (distance-- > 0)
        {
            if (c == 'a')
                rotate_a(list);
            else if (c == 'b')
                rotate_b(list);
        }
    else if (distance < 0)
        while (distance++ < 0)
        {
            if (c == 'a')
                rev_rotate_a(list);
            else if (c == 'b') 
                rev_rotate_b(list);
        }
    return ;
}


//-------------ALGORITHME--------------------

//choix de l'algorithme
void algo_choice(int argc, t_list *list_a, t_list *list_b)
{
    if (list_a->size <= 6)
    {
        if (argc == 3)
            sort_two_args(list_a);
        else if (argc == 4)
            sort_three_args(list_a);
        else if (argc == 5)
            sort_four_args(list_a, list_b);
        else if (argc == 6)
            sort_five_args(list_a, list_b);
        else if (argc == 7)
            sort_six_args(list_a, list_b);
    }
    else
    {
        sort_hundred_args(list_a, list_b);
    }
}

void sort_two_args(t_list *list)
{
    if (ft_check_sort_index(list) == 0)
        return ;
    swap_sa(list);
}

void sort_three_args(t_list *list)
{
    if (ft_check_sort_index(list) == 0)
        return ;
    if (get_head_i(list) == ft_find_smallest_index(list))
    {
        rotate_a(list);
        swap_sa(list);
        rev_rotate_a(list);
        return ;
    }
    if (get_tail_i(list) == ft_find_biggest_index(list))
        swap_sa(list);
    else if (get_head_i(list) == ft_find_biggest_index(list))
    {
        if (get_tail_i(list) == ft_find_smallest_index(list))
        {
            rotate_a(list);
            swap_sa(list);
        }
        else
            rotate_a(list);
    }
    
    else
        rev_rotate_a(list);
    return ;
}

void sort_four_args(t_list *list_a, t_list *list_b)
{
    if (ft_check_sort_index(list_a) == 0)
        return ;
    bring_to_head(list_a, ft_find_smallest_index(list_a), 'a');
    push_a_to_b(list_a, list_b);
    sort_three_args(list_a);
    push_b_to_a(list_a, list_b);
    return ;
}

void sort_five_args(t_list *list_a, t_list *list_b)
{
    if (ft_check_sort_index(list_a) == 0)//liste triée
        return ;
    bring_to_head(list_a, ft_find_smallest_index(list_a), 'a');
    push_a_to_b(list_a, list_b);
    sort_four_args(list_a, list_b);
    push_b_to_a(list_a, list_b);
    return ;
}

void sort_six_args(t_list *list_a, t_list *list_b)
{
    int smallest;
    
    smallest = 1;
    if (ft_check_sort_index(list_a) == 0)//liste triée
        return ;
    while (smallest <= 3)
    {
        bring_to_head(list_a, smallest, 'a');
        push_a_to_b(list_a, list_b);
        smallest++;
    }

    sort_three_args(list_a);
    sort_three_args(list_a);
    while (list_b->head)
        push_b_to_a(list_a, list_b);
    return ;
}

void sort_hundred_args(t_list *list_a, t_list *list_b)
{
    if (ft_check_sort_index(list_a) == 0)//liste triée
        return ;
    pushed_to_b(list_a, list_b);
    come_back(list_a, list_b);
    return ;
}

void pushed_to_b(t_list *list_a, t_list *list_b)
{
    int median;
    int mid_median;
    t_element	*current;

    median = (ft_find_smallest_index(list_a) + ft_find_biggest_index(list_a)) / 3;
    mid_median = (median + ft_find_smallest_index(list_a)) / 2;
    while (list_a->size > 4)
    //while (list_a->size > 0)
    {
        //cherche si 1 element de list_a est entre 0 et <= median
        while (ft_search_if_limit_in_list(list_a, median) == 1)
        {
            current = list_a->head;
            if (current->index <= median)
            {
                push_a_to_b(list_a, list_b);
                if (current->index < mid_median && list_b->size > 1)
                    rotate_b(list_b);
                                   //ft_display_stacks_data_index(list_a, list_b);
                //break;              
            }
            else
            {
                move_shortest_path_until(list_a, median);
                                        //ft_display_stacks_data_index(list_a, list_b);

            }
        }
        median = (ft_find_smallest_index(list_a) + ft_find_biggest_index(list_a)) / 2;
        mid_median = (median + ft_find_smallest_index(list_a)) / 2;
        
    }
    while (list_a->size != 0)
        push_a_to_b(list_a, list_b);
}

void come_back(t_list *list_a, t_list *list_b)
{

    while (list_b->head)
    {
      bring_to_head(list_b, list_b->size, 'b');
      push_b_to_a(list_a, list_b);
    }
        
}

//-----------MAIN--------------
int main(int argc, char **argv)
{
    t_list *list_b;
    t_list *list_a ;

    if (argc == 1)
        //return (0);
    check_argument(argc, argv);
    list_a = creat_final_list_a(argc, argv);
    /*if (ft_check_sort_data(list_a) == 0)
    {
        ft_free_list(list_a);
        return (0);
    }*/
    if (argc >= 2)
    {
        list_b = ft_creat_list();
        if (!list_a || !list_b)
        {
            ft_free_list(list_a);
            ft_free_list(list_b);
            return (1);
        }
        algo_choice(argc, list_a, list_b);
    }
    ft_free_list(list_a);
    ft_free_list(list_b);
    return (0);
}



//---------------  10h15 --------------


//      valgrind --leak-check=full --show-leak-kinds=all ./a.out

//      gcc -g -Wall -Wextra -Werror test.c
