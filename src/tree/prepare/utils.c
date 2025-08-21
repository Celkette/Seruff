/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:43:32 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/13 17:45:26 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Retourne le prochain token inutilise dans l'arbre et change status a utilise
t_token	*consume_token(t_list *lst)
{
	t_token	*tmp;

	if (!lst)
		return (0);
	while (lst)
	{
		tmp = (t_token *)lst->data;
		if (!tmp->consume)
		{
			tmp->consume++;
			return (tmp);
		}
		lst = lst->next;
	}
	return (0);
}

// Retourne le prochain token inutilise dans l'arbre sans change status
t_token	*see_token(t_list *lst)
{
	t_token	*tmp;

	if (!lst)
		return (0);
	while (lst)
	{
		tmp = (t_token *)lst->data;
		if (!tmp->consume)
			return (tmp);
		lst = lst->next;
	}
	return (0);
}
