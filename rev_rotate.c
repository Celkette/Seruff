/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rev_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:58:41 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:58:43 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
