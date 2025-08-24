/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:11:45 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/21 12:37:20 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	update_env_oldpath(t_env *minishell)
{
	t_list	*lst;
	t_var	*tmp;

	if (!minishell)
		return ;
	lst = minishell->env_list;
	while (lst)
	{
		if (!lst->data)
			return ;
		tmp = (t_var *)lst->data;
		if (ft_strlen(tmp->key) == 6 && ft_strncmp(tmp->key, "OLDPWD", 6) == 0)
		{
			free(tmp->val);
			tmp->val = ft_strndup(minishell->last_path,
					ft_strlen(minishell->last_path));
			if (!tmp->val)
				return ;
		}
		lst = lst->next;
	}
}

int	make_chdir(t_arg *arg, char *new_path)
{
	if (!new_path)
		return (1);
	else if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg->node->arg->data, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	return (0);
}
