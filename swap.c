#include "push_swap.h"

void	ps_swap(t_list *list)
{
	t_element	*first;
	t_element	*second;

	if (!list || list->head == NULL || list->head->next == list->head)
		return ;
	first = list->head;
	second = first->next;
	if (list->head->next == list->tail)
		list->head = second;
	else
	{
		second->next->prev = first;
		first->next = second->next;
		first->prev = second;
		second->next = first;
		second->prev = list->tail;
		list->head = second;
		list->tail->next = list->head;
		list->head->prev = list->tail;
	}
	return ;
}

void	swap_sa(t_list *list_a)
{
	ps_swap(list_a);
	write(1, "sa\n", 3);
	return ;
}

void	swap_sb(t_list *list_b)
{
	ps_swap(list_b);
	write(1, "sb\n", 3);
	return ;
}

void	swap_ss(t_list *list_a, t_list *list_b)
{
	ps_swap(list_a);
	ps_swap(list_b);
	write(1, "ss\n", 3);
	return ;
}
