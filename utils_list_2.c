#include "push_swap.h"

void ft_indexage_list(t_list *list)
{
    int i;

    i = 1;
    while (i <= list->size)
    {
        list->head->index = i;
        list->head = list->head->next;
        i++;
    }
}

t_list *ft_copy_list(t_list *list_a)
{
    t_list *list_c;
    t_element *current_a;
    int i;

    if (!list_a || list_a->head == NULL)
        return (NULL);
    list_c = ft_creat_list();
    current_a = list_a->head;
    i = 0;
    while (i < list_a->size)
    {
        ft_creat_and_init_copy_tail(list_c, current_a);
        current_a = current_a->next;
        list_c->size++;
        i++;
    }
    return (list_c);
}

void ft_swap_data(t_element *a, t_element *b)
{
    long temp = a->data;
    a->data = b->data;
    b->data = temp;
}


void ft_free_list(t_list *list)
{
    t_element *current;
    t_element *next;

    if (list == NULL)
        return;
    if (list->head == NULL)
        return (free(list));
    current = list->head;
    if (current->next == list->head)
    {
        free(current);
        free(list);
        return;
    }
    while (current->next != list->head)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(current);
    free(list);
}
