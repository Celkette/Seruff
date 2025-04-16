/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_algo_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:57:19 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:57:22 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
