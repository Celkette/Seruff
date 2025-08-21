/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:12:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/11 05:12:29 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	del_node(t_list **head, t_list *lst)
{
	if (!lst || !head || !*head)
		return (-1);
	if (lst == *head)
	{
		*head = lst->next;
		if ((*head)->prev)
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

static void	del_env_var(t_arg *arg, t_list *lst, int *code)
{
	if (!arg || !lst || !code)
		return ;
	while (lst && *code != -1)
	{
		if (ft_strncmp(((t_var *)lst->data)->key, \
			(char *)arg->node->arg->data, \
			ft_strlen((char *)arg->node->arg->data) + 1) == 0)
		{
			*code = del_node(&arg->minishell->env_list, lst);
			break ;
		}
		lst = lst->next;
	}
}

int	unset_built_in(void *p)
{
	t_arg	*arg;
	t_list	*lst;
	int		code;

	if (!p)
		return (-1);
	code = 0;
	arg = (t_arg *)p;
	if (!arg->minishell || !arg->node || !arg->node->arg)
	{
		free(p);
		return (-1);
	}
	if (arg->minishell->env_list)
		del_env_var(arg, arg->minishell->env_list, &code);
	lst = is_var_in_local(arg->minishell, (char *)arg->node->arg->data);
	if (lst && code != -1)
		arg->minishell->var = ft_delete_node_var(lst);
	free(p);
	return (code);
}
