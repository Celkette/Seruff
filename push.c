#include "push_swap.h"

void ft_init_head_list(t_list *list, t_element *add)
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

void ft_point_out_head_list(t_list *list)
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

void push_b_to_a(t_list *list_a, t_list *list_b)
{
    ps_push(list_b, list_a);
    write(1, "pa\n", 3);
}

void push_a_to_b(t_list *list_a, t_list *list_b)
{
    ps_push(list_a, list_b);
    write(1, "pb\n", 3);
}
