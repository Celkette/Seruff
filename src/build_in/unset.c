/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:12:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 14:50:17 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	del_node(t_list **head, t_list *lst)
{
	if (!lst)
		return (-1);
	if (lst == *head)
	{
		*head = lst->next;
		(*head)->prev = NULL;
	}
	else
	{
		if (lst->next)
			lst->next->prev = lst->prev;
		if (lst->prev)
			lst->prev->next = lst->next;
	}
	free(((t_var *)lst->data)->key);
	free(((t_var *)lst->data)->val);
	free(lst->data);
	free(lst);
	return (0);
}

int	unset_built_in(void *p)
{
	t_arg	*arg;
	t_list	*lst;

	if (!p)
		return (-1);
	arg = (t_arg *)p;
	if (!arg->minishell || !arg->node || !arg->node->arg)
	{
		free(p);
		return (-1);
	}
	lst = arg->minishell->env_list;
	while (lst)
	{
		if (ft_strncmp(((t_var *)lst->data)->key, (char *)arg->node->arg->data, ft_strlen((char *)arg->node->arg->data) + 1) == 0)
			return (del_node(&arg->minishell->env_list, lst));
		lst = lst->next;
	}
	free(p);
	return (0);
}
