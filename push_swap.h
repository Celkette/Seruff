/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:59:45 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:59:47 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
 #define PUSH_SWAP_H

#include "libft/libft.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

//--------- Version 1 ---------//
//struc d'un noeud de la liste
typedef struct  s_element
{
	int		    	data;      //  valeur du noeud
    int				    index;     // indexation de la liste
	struct s_element	*next;
    struct s_element	*prev;     //si doublement chainée
}	           t_element;

typedef struct  s_list
{
    struct  s_element   *head;       // tete de la liste
    struct  s_element   *tail;       // queue de la liste
    int                 size;      // Nombre d'éléments dans la liste
    //s_element   *head;
}               t_list;

//PUSH
void    ft_init_head_list(t_list *list, t_element *add);
void    ft_point_out_head_list(t_list *list);
void    ps_push(t_list *list_a, t_list *list_b);
void    push_b_to_a(t_list *list_b, t_list *list_a);
void    push_a_to_b(t_list *list_a, t_list *list_b);
// SWAP
void    ps_swap(t_list *list);
void    swap_sa(t_list *list);
void    swap_sb(t_list *list_b);
void    swap_ss(t_list *list_a, t_list *list_b);
// ROTATE
void ps_rotate(t_list *list);
void rotate_a(t_list *list_a);
void rotate_b(t_list *list_b);
void rotate_a_and_b(t_list *list_a, t_list *list_b);

//REVERSE ROTATE
void rev_rotate(t_list *list);
void rev_rotate_a(t_list *list_a);
void rev_rotate_b(t_list *list_b);
void rev_rotate_a_and_b(t_list *list_a, t_list *list_b);


//Check argument

int ft_args_isdigit(char *str);
int ft_is_space_inside(const char *str);
int check_max_min(char *n);
int check_double_if_string_arg(char **array);
int check_nb_if_args(int argc, char **argv);
int check_string_or_arg(int argc, char **argv);
int check_argument(int argc, char **argv);//appel toutes les autres = MAIN

//creation list_a et indexation elements
void    ft_init_element_tail(t_list *list, t_element *add);
void    ft_indexage_list(t_list *list_c);
void    ft_swap_data(t_element *a, t_element *b);
void    ft_copy_index(t_list *list_c, t_list *list_a);
void    ft_free_list(t_list *list);
void    ft_sort_list_c(t_list *list);
void ft_swap_data(t_element *a, t_element *b);
t_list  *ft_creat_list(void);
t_list  *creat_list_a(int argc, char **argv);
t_list  *ft_copy_list(t_list *list_a);
t_list  *creat_final_list_a(int argc, char **argv);
void   ft_index_list(t_list *list);
t_list  *creat_list_a_from_args(int argc, char **argv);
t_element   *ft_creat_element(void);
t_element   *ft_creat_and_init_copy_tail(t_list *list_c, t_element *current_a);

//utils Algorithme

int	ft_check_sort_data(t_list *list);//
int	ft_check_sort_index(t_list *list);//
int find_gap_index(t_list *list);//
int	ft_find_smallest_index(t_list *list);//
int	ft_find_biggest_index(t_list *list);//
int	get_head_i(t_list *list);//
int	get_tail_i(t_list *list);//
int count_path_ahead(t_list *list, int target_i);//
int count_path_back(t_list *list, int target_i);//
int find_shortest_path(t_list *list, int target_i);//

int count_path_ahead_until(t_list *list, int limit);
int count_path_back_until(t_list *list, int limit);
void move_shortest_path_until(t_list *list_a, int limit);
int	ft_search_if_limit_in_list(t_list *list, int limit);
void	bring_to_head(t_list *list, int target_i, char c);



//Algorithme
void algo_choice(int argc, t_list *list_a, t_list *list_b);
void sort_two_args(t_list *list);
void sort_three_args(t_list *list);
void sort_four_args(t_list *list_a, t_list *list_b);
void sort_five_args(t_list *list_a, t_list *list_b);
void sort_six_args(t_list *list_a, t_list *list_b);
void sort_hundred_args(t_list *list_a, t_list *list_b);
void pushed_to_b(t_list *list_a, t_list *list_b);
void come_back(t_list *list_a, t_list *list_b);


//a supprimer
t_list *creat_list_a_from_string(char **argv);





#endif
