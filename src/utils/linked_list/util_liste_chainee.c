/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_liste_chainee.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:42:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/25 10:49:42 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/*//@Create a new node for a double-linked liste with the given data
// @param data The data to be stored in the node
// @return A pointer to the newly created node, or NULL if data is NULL
t_list	*ft_create_node(void *data)//cree un noeud
{
	t_list	*new_node;

	if (data == NULL)//si pointeur vers rien
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

t_list *ft_add_node(t_list *list, void *data)
{
	t_list *new_add;					//head = tête de liste (ou NULL)
	t_list *current;					//current = parcourt la liste
										//new_add = nouveau nœud créé qu'on veut ajouter
	if (data == NULL)//si pointeur vers rien					
		return (NULL);
	new_add = ft_create_node(data);// cree un nouveau noeud
	if (new_add == NULL)
		return (NULL);

	if (list == NULL)// Si pas de liste, le nouveau noeud devient la tete
	{
		return (new_add);// new_add devient le 1er et seul noeud de la liste
	}
	current = list; // current pointe sur head (head  pointe toujours vers le 1er nœud)
	while (current->next != NULL)
		current = current->next;// On va à la fin de liste
	current->next = new_add; // Ajoute le nouveau noeud a la fin
	//new_add->prev = current; //seulement si doublement chainée
	new_add->next = NULL; // Le nouveau noeud devient le dernier de la liste
	return (list); // On retourne la vraie tete de liste
}
t_list *ft_delete_node(t_list *list, void *data)
{
	t_list *current;//noeud de la liste qu'on veut supprimer
	t_list *current_prev;//noeud precedent de current	

	if (!list || !data)//data est le contenu du noeud a supprimer
		return (list);// retourne la tete de liste si vide ou data == NULL
// Cas 1: Suppression du premier nœud
	if (list->data == data)
	{
		current = list->next;// current pointe sur le 2eme noeud
		free(list);//libere le 1er noeud
		return (current);// retourne le 2eme noeud qui devient la tete de liste	
	}
// Cas 2: Suppression d'un nœud au milieu/fin
	current_prev = list;// current_prev pointe sur le 1er noeud
	while (current_prev->next && current_prev->next->data != data)//si n'est pas NULL et noeud suivant n'est pas le noeud a supprimer
		current_prev = current_prev->next;// on avance dans la liste
	//on est sortie du while donc soit noeud suivant == null soit ->data == data
	if (current_prev->next)//si noeud suivant != NULL alors ->data == data, c'est le noeud à supprimer
	{
		current = current_prev->next;
		current_prev->next = current->next;//current_prev pointe sur le noeud suivant de current
		free(current);
	}
	else//si on arrive ici, c'est que le noeud n'a pas été trouvé
		perror("Node not found in delete_node");//si on arrive ici, c'est que le noeud n'a pas été trouvé
	return (list);// retourne la tete de liste
}*/


/*void	ft_free_list(t_list *list)
{
	t_list	*current;
	t_element	*next;

	if (list == NULL)
		return ;
	if (list->head == NULL)
		return (free(list));
	current = list->head;
	if (current->next == list->head)
	{
		free(current);
		free(list);
		return ;
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
void	ft_point_out_head_list(t_list *list)
{
	t_element	*first;

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
}*/
//Libère la mémoire de l’élément passé en argument en
//utilisant la fonction ’del’ puis avec free(3). La
//mémoire de ’next’ ne doit pas être free.


/*
// N'oubliez pas de libérer la mémoire !
    free(((t_token *)my_list_node->data)->value);
    free(my_token);
    free(my_list_node);	*/

/**********************************
*               MAIN              *
***********************************/
//teste t_liste *add_node(t_liste *liste, void *data)
/*int main(int argc, char **argv)
{
	t_liste *liste = NULL;
	int i;

	if (argc < 2)
	{
		printf("Usage: %s <data1> <data2> ...\n", argv[0]);
		return (1);
	}

	for (i = 1; i < argc; i++)
	{
		liste = add_node(liste, argv[i]);
		if (liste == NULL)
		{
			printf("Memory allocation failed.\n");
			return (1);
		}
	}

	t_liste *current = liste;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->data);
		current = current->next;
	}

	return 0;
}*/



