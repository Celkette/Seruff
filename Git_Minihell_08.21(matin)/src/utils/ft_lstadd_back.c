/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 07:20:28 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/03 07:22:49 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	ft_lstadd_back(t_list **lst, t_list *neww)
{
	if (lst == NULL || neww == NULL)
		return ;
	else if (*lst == NULL)
	{
		*lst = neww;
		return ;
	}
	else if ((*lst)->next != NULL)
		return (ft_lstadd_back(&(*lst)->next, neww));
	(*lst)->next = neww;
	neww->prev = (*lst);
}
