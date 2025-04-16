#include "push_swap.h"

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
