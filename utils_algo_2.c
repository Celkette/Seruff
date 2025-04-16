/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_algo_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:57:31 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:57:33 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
