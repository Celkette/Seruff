/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 00:04:47 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/22 01:14:51 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	ft_lstadd_front(t_list **lst, t_list *neww)
{
	if (!neww)
		return ;
	if (lst && *lst)
	{
		(*lst)->prev = neww;
		neww->next = *lst;
	}
	*lst = neww;
}
