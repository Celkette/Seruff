/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:42:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/29 13:52:14 by cle-rouz         ###   ########.fr       */
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

	if (data == NULL) //si pointeur vers rien
		return (NULL);
	new_node = malloc(sizeof(t_list));//alloc nveau noeud
//VERIFIER LIBERATION PLUS TARD DE CE MALLOC
	if (new_node == NULL)
	{
		perror("Memory allocation error for create_node");
		return (NULL);//permet le free_token_list dans tokenise the hell
	}
	ft_memset(new_node, 0, sizeof(t_list));//met tous les champs a 0
	new_node->data = data;//stocke data dans le noeud(qui sera = t_token)
	return (new_node);
}

/*****************************************************************
*	Ajout d'un nouveau noeud en fin de liste	                 *
* !!! la liste se finit toujours par un element null
*		-appelle ft_create_node (cree un nouveau noeud de liste) *
******************************************************************/
//@Ajout d'un nouveau noeud à la fin de la liste
// @param list La liste dans laquelle ajouter le noeud
// @param data La donnée à stocker dans le nouveau noeud
// @return La liste mise à jour avec le nouveau noeud ajouté
t_list	*ft_add_node(t_list *list, void *data)
{
	t_list	*current;					//current = parcourt la liste
	t_list	*new_add;					//head = tête de liste (ou NULL)
										//new_add = nouveau nœud créé qu'on veut ajouter
	if (data == NULL) //si pointeur vers rien					
		return (NULL);
	new_add = ft_create_node(data);// cree un nouveau noeud
	if (new_add == NULL)
		return (NULL);
	if (list == NULL) // Si pas de liste, le nouveau noeud devient la tete
	{
		return (new_add);// new_add devient le 1er et seul noeud de la liste
	}
	current = list;// current pointe sur head (head  pointe toujours vers le 1er nœud)
	while (current->next != NULL)
		current = current->next;// On va à la fin de liste
	current->next = new_add;// Ajoute le nouveau noeud a la fin
	new_add->prev = current;//seulement si doublement chainée
	new_add->next = NULL;// Le nouveau noeud devient le dernier de la liste
	return (list);// On retourne la vraie tete de liste
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
	while (last_add->next) //cherche dernier element de add_list
		last_add = last_add->next;
	//liaison de [add_list]->[current->next]
	last_add->next = list;//suite de add_list est list, même si list==null
	if (list)
	list->prev = last_add;
	return (add_list); // Nouvelle tête
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
	next = NULL;//si list est vide, ou current->next n'existe pas, next est NULL
	if (current && current->next) //si list existe et current->next != NULL
		next = current->next; // Sauvegarde le suivant de current
	last_add = add_list;
	while (last_add->next) //cherche dernier element de add_list
		last_add = last_add->next;
	//liaison de [current]->[add_list]->[current->next]
	last_add->next = next;//même si null, suivant du total add_list == ancien current->next
	if (next)			//si ancien current->next != null
		next->prev = last_add;// ancien current->next->prev == dernier de add_list
	current->next = add_list;
	add_list->prev = current;
	return (list);
}


/************************************************
*	Suppression d'un noeud de la liste	        *
* 		Cree en doublement chainée	            *
*à partir du pointeur sur l'element a supprimer *
* Libere le noeud supprimé
*************************************************/
//@Suppression d'un noeud de la liste doublement chainée
// @param node Le noeud à supprimer
// @return Le noeud précédent si possible, sinon le suivant, sinon NULL
// +flag del pour savoir si on a supprimé -1 le noeud suivant ou 1 le noeud precedent
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
		prev->next = next;//si next==NULL, prev->next reste NULL
		if (next)
			next->prev = prev;
		free_token_node(node);
		return (prev);
	}
	if (next)
	{
		next->del = -1;
		next->prev = prev;//si prev==NULL, next->prev reste NULL 
		//mettre a jour la tete de liste dans la fonction appellante si !prev
		free_token_node(node);
		return (next);
	}
	free_token_node(node);
	return (NULL);
}
