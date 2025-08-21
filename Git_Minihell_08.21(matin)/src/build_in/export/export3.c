/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:10:23 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/19 13:16:07 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	print_export_no_arg(t_list *lst)
{
	t_var	*var;

	while (lst)
	{
		var = (t_var *)lst->data;
		if (var)
		{
			ft_putstr_fd("export ", 1);
			if (var->key)
				ft_putstr_fd(var->key, 1);
			ft_putstr_fd("=\"", 1);
			if (var->val)
				ft_putstr_fd(var->val, 1);
			ft_putstr_fd("\"\n", 1);
		}
		lst = lst->next;
	}
}

int	export_no_arg(t_env *minishell)
{
	t_list	*lst;

	if (!minishell)
		return (-1);
	if (!minishell->env_list)
		return (0);
	lst = minishell->env_list;
	print_export_no_arg(lst);
	return (0);
}
