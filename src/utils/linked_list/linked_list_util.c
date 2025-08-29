/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:42:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/25 00:10:11 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**********************************************************
*		Creation d'un nouveau noeud		                  *
*		-appelle ft_memset -> init à zero le nveau noeud  *
***********************************************************/
//@Create a new node for a double-linked liste with the given data
// @param data The data to be stored in the node
// @return A pointer to the newly created node, or NULL if data is NULL
t_list	*ft_create_node(void *data)
{
	t_list	*new_node;

	if (data == NULL)
		return (NULL);
	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
	{
		perror("Memory allocation error for create_node");
		return (NULL);
	}
	ft_memset(new_node, 0, sizeof(t_list));
	new_node->data = data;
	return (new_node);
}

/*****************************************************************
*	Ajout d'un nouveau noeud en fin de liste t_var	                 *
* !!! la liste se finit toujours par un element null
*		-appelle ft_create_node (cree un nouveau noeud de liste) *
******************************************************************/
//@Ajout d'un nouveau noeud à la fin de la liste
// @param list La liste dans laquelle ajouter le noeud
// @param data La donnée à stocker dans le nouveau noeud
// @return La liste mise à jour avec le nouveau noeud ajouté
t_list	*ft_add_node(t_list *list, void *data)
{
	t_list	*current;
	t_list	*new_add;

	if (data == NULL)
		return (NULL);
	new_add = ft_create_node(data);
	if (new_add == NULL)
		return (NULL);
	if (list == NULL)
	{
		return (new_add);
	}
	current = list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_add;
	new_add->prev = current;
	new_add->next = NULL;
	return (list);
}

/**********************************************************
 * Logique pour integrer une liste de noeud en tete  
* d'une autre liste                 *
*		Ne crée pas de nouveaux nœuds
*		Relie directement les nœuds de add_list à la liste principale.
* Au final list n'existe plus 
***********************************************************/
t_list	*ft_add_list_at_begin(t_list *list, t_list *add_list)
{
	t_list	*last_add;

	if (!list || !add_list)
		return (NULL);
	last_add = add_list;
	while (last_add->next)
		last_add = last_add->next;
	last_add->next = list;
	if (list)
		list->prev = last_add;
	return (add_list);
}

/**********************************************************
* Logique pour integrer une liste de noeud au milieu 
* d'une autre liste                 *
*		Ne crée pas de nouveaux nœuds
*		Relie directement les nœuds de add_list à la liste principale.
* Au final add_list n'existe plus 
***********************************************************/
t_list	*ft_add_list_at_middle(t_list *list, t_list *add_list, t_list *current)
{
	t_list	*last_add;
	t_list	*next;

	if (!list || !add_list || !current)
		return (NULL);
	next = NULL;
	if (current && current->next)
		next = current->next;
	last_add = add_list;
	while (last_add->next)
		last_add = last_add->next;
	last_add->next = next;
	if (next)
		next->prev = last_add;
	current->next = add_list;
	add_list->prev = current;
	return (list);
}

/************************************************
*	Suppression d'un noeud t_token de la liste  *
* 		Cree en doublement chainée	            *
*à partir du pointeur sur l'element a supprimer *
* Libere le noeud supprimé
*************************************************/
//@Suppression d'un noeud de la liste doublement chainée
// @param node Le noeud à supprimer
// @return Le noeud précédent si possible, sinon le suivant, sinon NULL
// +flag del pour savoir si on a supprimé -1 le noeud suivant 
// ou 1 le noeud precedent
t_list	*ft_delete_node(t_list *node)
{
	t_list	*prev;
	t_list	*next;

	if (!node)
		return (NULL);
	prev = node->prev;
	next = node->next;
	if (prev)
	{
		prev->del = 1;
		prev->next = next;
		if (next)
			next->prev = prev;
		free_token_node(node);
		return (prev);
	}
	if (next)
	{
		next->del = -1;
		next->prev = prev;
		free_token_node(node);
		return (next);
	}
	free_token_node(node);
	return (NULL);
}
