#include "push_swap.h"

void error(char *message)
{
    ft_printf("\033[31m*** %s ***\e[0m\n", message);
}
//gérer le cas où moins de six nombres sont donnés dans la pile A puis le cas +de 6 nbr
//doit fonctionner avec plusieurs arguments numériques
// ----- ./push_swap 1 3 5 +9 20 -4 50 60 04 08
// ou avec une liste de caractères en tant que paramètre
// ----- ./push_swap "3 4 6 8 9 74 -56 +495"

// Ne doit pas fonctionner d'autre caractere
ft_is_digit(char *str)// si OK = 1 si KO = 0
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
            i++;
        if ((str[i] == '-' || str[i] == '+') && (str[i + 1] >= '0' || str[i + 1] <= '9'))
            i++;
        if (str[i] >= '0' || str[i] <= '9')
            i++;
        else
        {
            error("Error: Invalid character");
            return (0);
        }
    }
    return (1);
}

// Ne doit pas fonctionner si double
//  3 & 03    
// doit fonctionner si 3 & -3

//doit fonctionner avec INT MAX et INT MIN (2147483647 & -2147483648)

//Rien n'est spécifié lorsque des chaînes et des entiers sont mélangés
--> quel difference dans la gestion
//

//
//swap :  échangez les 2 premiers éléments en haut de la pile
void swap(t_element **head)
{
    t_element *first;
    t_element *second;

    first = head;
    second = first->next;
    if (*head == NULL || (*head)->next == NULL)
        return ();
    first->next = second->next;
    second->next = first;
    *head = second;
}
// sa
void swap_sa(t_element **head_a)
{
    swap(head_a);
}
// sb
void swap_sb(t_element **head_b)
{
    swap(head_b);
}
// ss = sa + sb
void swap_ss(t_element **head_a, t_element **head b)
{
    swap(head_a);
    swap(head_b);
}
//
// push :  prend le premier élément de la pile a et le place sur la pile b
void push(t_element **head_x, t_element **head_z)
{
    t_element *temp;

    if (*head_x == NULL)
        return ();
    temp = *head_x;
    *head_x = (*head_x)->next;
    temp->next = *head_z;
    *head_z = temp;
}
// pa
void push_pa(t_element **head_a, t-element **head_b)
{
    push(head_a, head_b);
}
// pb
void push_pb(t_element **head_b, t-element **head_a)
{
    push(head_b, head_a);
}
//
// rotate : Décale Le premier élément devient le dernier.
void rotate(t_element **head)
{
    t_element *temp_first;
    t_element *temp_last;

    temp_first = *head;
    temp_last = *head;
    if (*head == NULL || (*head)->next == NULL)
        return ();
    while (temp_last->next != NULL)
    {
        temp_last = temp_last->next;
    }
    *head = (*head)->next;
    temp_last->next = temp_first;
    temp_first->next = NULL;
}
// ra
void rotate_ra(t_element **head_a)
{
    rotate(head_a);
}
// rb
void rotate_rb(t_element **head_b)
{
    rotate(head_b);
}
// rr = ra + rb
void rotate_rr(t_element **head_a, t_element **head_b)
{
    rotate(head_a);
    rotate(head_b);
}
//
// reverse rotate : Décale Le dernier élément devient le premier.
void reverse_rotate(t_element **head)
{
    t_element *temp_last;
    t_element *temp_bef_last;

    temp_bef_last = *head;
    if (*head == NULL || (*head)->next == NULL)
        return ();
    while (temp_bef_last->next->next != NULL)
    {
        temp_bef_last = temp_bef_last->next;
    }
    temp_last = temp_bef_last->next;
    temp_bef_last->next = NULL;
    temp_last->next = *head;
    *head = temp_last;
}
// rra
void reverse_rotate_rra(t_element **head_a)
{
    rotate(head_a);
}
// rrb
void reverse_rotate_rrb(t_element **head_b)
{
    rotate(head_b);
}
// rrr = rra + rrb
void reverse_rotate_rrr(t_element **head_a, t_element **head_b)
{
    rotate(head_a);
    rotate(head_b);
}

// fonctions pour manipuler la liste N°1
// Création de noeuds

// fonctions pour manipuler la liste N°2
// Ajout d'éléments à la liste






















