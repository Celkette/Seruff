/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:43:35 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 14:08:08 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int env(void *p)
{
	t_env	*minishell;
	t_list	*lst;
	t_var	*var;

	if (!p)
		return (-1);
	minishell = ((t_arg *)p)->minishell;
	if (!minishell->env_list)
	{
		free(p);
		return (-1);
	}
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		printf("%s=%s\n", var->key, var->val);
		lst = lst->next;
	}
	free(p);
	return (0);
}
