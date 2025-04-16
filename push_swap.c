#include "push_swap.h"

void sort_hundred_args(t_list *list_a, t_list *list_b)
{
    if (ft_check_sort_index(list_a) == 0)//liste triée
        return ;
    pushed_to_b(list_a, list_b);
    come_back(list_a, list_b);
    return ;
}

void algo_choice(int argc, t_list *list_a, t_list *list_b)
{
    if (list_a->size <= 6)
    {
        if (argc == 3)
            sort_two_args(list_a);
        else if (argc == 4)
            sort_three_args(list_a);
        else if (argc == 5)
            sort_four_args(list_a, list_b);
        else if (argc == 6)
            sort_five_args(list_a, list_b);
        else if (argc == 7)
            sort_six_args(list_a, list_b);
    }
    else
    {
        sort_hundred_args(list_a, list_b);
    }
}

void pushed_to_b(t_list *list_a, t_list *list_b)
{
    int median;
    int mid_median;
    t_element	*current;

    median = (ft_find_smallest_index(list_a) + ft_find_biggest_index(list_a)) / 3;
    mid_median = (median + ft_find_smallest_index(list_a)) / 2;
    while (list_a->size > 4)
    {
        //cherche si 1 element de list_a est entre 0 et <= median
        while (ft_search_if_limit_in_list(list_a, median) == 1)
        {
            current = list_a->head;
            if (current->index <= median)
            {
                push_a_to_b(list_a, list_b);
                if (current->index < mid_median && list_b->size > 1)
                    rotate_b(list_b);
                                   //ft_display_stacks_data_index(list_a, list_b);
                //break;              
            }
            else
            {
                move_shortest_path_until(list_a, median);
                                        //ft_display_stacks_data_index(list_a, list_b);

            }
        }
        median = (ft_find_smallest_index(list_a) + ft_find_biggest_index(list_a)) / 2;
        mid_median = (median + ft_find_smallest_index(list_a)) / 2;
        
    }
    while (list_a->size != 0)
    {
        push_a_to_b(list_a, list_b);
    }
}

void come_back(t_list *list_a, t_list *list_b)
{

    while (list_b->head)
    {
      bring_to_head(list_b, list_b->size, 'b');
      push_b_to_a(list_a, list_b);
    }
        
}

//-----------MAIN--------------
int main(int argc, char **argv)
{
    t_list *list_b;
    t_list *list_a ;

    if (argc == 1)
        //return (0);
    check_argument(argc, argv);
    list_a = creat_final_list_a(argc, argv);
    /*if (ft_check_sort_data(list_a) == 0)
    {
        ft_free_list(list_a);
        return (0);
    }*/
    if (argc >= 2)
    {
        list_b = ft_creat_list();
        if (!list_a || !list_b)
        {
            ft_free_list(list_a);
            ft_free_list(list_b);
            return (1);
        }
        algo_choice(argc, list_a, list_b);
    }
    ft_free_list(list_a);
    ft_free_list(list_b);
    return (0);
}