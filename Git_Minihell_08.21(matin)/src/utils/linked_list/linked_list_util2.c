/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_util2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:31:55 by celine            #+#    #+#             */
/*   Updated: 2025/08/17 13:49:40 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/****************************************
*   Gestion des listes chainée t_var    *
****************************************/

/************************************************
*	Suppression d'un noeud t_var de la liste 	*
* 		Cree en doublement chainée	            *
*à partir du pointeur sur l'element a supprimer *
* Libere le noeud supprimé                      *
*************************************************/
t_list	*ft_delete_node_var(t_list *node)
{
	t_list	*prev;
	t_list	*next;
	t_list	*head;

	head = NULL;
	if (!node)
		return (NULL);
	prev = node->prev;
	next = node->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (prev)
		head = prev;
	else if (next)
		head = next;
	free_var_node(node);
	while (head && head->prev)
		head = head->prev;
	return (head);
}

/************************************************
*	Suppression d'un noeud t_token de la liste  *
* 		Cree en doublement chainée	            *
*à partir du pointeur sur l'element a supprimer *
* Libere le noeud supprimé et renvoie le next   *    
*************************************************/
//IDENTIQUE a ft_delete_node mais return(next) ou (NULL)
//@Suppression d'un noeud de la liste doublement chainée
// @param node Le noeud à supprimer
// @return Le noeud suivant, sinon NULL
// +flag del pour savoir si on a supprimé -1 le noeud suivant 
// ou 1 le noeud precedent
t_list	*ft_delete_remove_token(t_list *node)
{
	t_list	*prev;
	t_list	*next;

	if (!node)
		return (NULL);
	prev = node->prev;
	next = node->next;
	if (prev)
	{
		prev->next = next;
	}
	if (next)
	{
		next->prev = prev;
	}
	free_token_node(node);
	if (!next)
		return (prev);
	if (prev || next)
		return (next);
	return (NULL);
}

t_list	*head_of_list(t_list *list)
{
	t_list	*current;

	if (!list)
		return (0);
	current = list;
	while (current && current->prev)
		current = current->prev;
	return (current);
}
