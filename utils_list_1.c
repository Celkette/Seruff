/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:56:28 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:56:30 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
