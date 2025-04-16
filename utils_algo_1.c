/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_algo_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:57:41 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:57:44 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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

int	get_head_i(t_list *list)
{
    t_element	*head;

    head = list->head;
    return (head->index);
}

int	get_tail_i(t_list *list)
{
    t_element	*tail;

    tail = list->tail;
    return (tail->index);
}

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
