/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:55:57 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:56:05 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_copy_index(t_list *list_c, t_list *list_a)
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


void	ft_index_list(t_list *list)
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


t_list	*creat_list_a_from_args(int argc, char **argv)
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

t_list	*creat_final_list_a(int argc, char **argv)
{
	t_list *list_a;
	if (argc >= 2)
	list_a = creat_list_a_from_args(argc, argv);
	ft_index_list(list_a);
	return (list_a);
}
