/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:25:36 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/31 10:14:45 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**************************************************
* logique pour recuperer la valeur d'une variable *
*		*-find_value_var()                        *
*			-appelle ft_strdup()				  *
*			-appelle is_var_in_local()			  *
*			-appelle ft_strlen()				  *
*			-appelle ft_strncmp()				  *
*			-appelle getenv()			     	  *
* Return valeur de la var ou ""                   *
***************************************************/

char	*find_value_var(t_env *minishell, t_list *list, char *var_name)
{
	t_list	*current;
	t_var	*var_str;

	if (!list || !var_name)
		return (NULL);
	
	if (is_var_in_local(list, var_name)) //Cherche dans l'environnement local
	{
		current = list;//current devient la tete de list
		while (current)
		{
			var_str = (t_var *)current->data;// Cast de current->data en t_var*
			if (var_str && var_str->key && ft_strlen(var_name)
				== ft_strlen(var_str->key) && ft_strncmp(var_name
					, var_str->key, ft_strlen(var_name)) == 0)
				return (var_str->val);// retourne pointeur sur valeur de la var trouvee (val)
				//return(ft_strdup(var_str->val));// retourne chaine= valeur de la var trouvee (val)
			current = current->next;
		}
	}
	if (is_var_in_env(minishell, var_name)) // Cherche dans l'environnement global
		return (var_env_value(minishell, var_name));// retourne pointeur sur valeur
		//return(getenv(var_name));// retourne chaine= valeur 
	return (ft_strdup("")); // Variable non trouvée
	/*if (is_var_in_env(minishell, var_name)) //Cherche dans l'env
		var_str->val = var_env_value(minishell, var_name);
	return (NULL); // Variable non trouvée*/
}
