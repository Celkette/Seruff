/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:58:54 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/16 15:58:56 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
