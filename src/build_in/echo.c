/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 05:23:52 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 05:53:51 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	echoo(void *p)
{
	t_arg	*arg;
	t_list	*lst;
	int		new_line;

	new_line = 1;
	if (!p)
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	arg = (t_arg *)p;
	if (arg->node && arg->node->arg)
	{
		lst = arg->node->arg;
		while (lst)
		{
			if (ft_strncmp((char *)lst->data, "-n", 2) != 0)
				break;
			new_line = 0;
			lst = lst->next;
		}
		while (lst)
		{
			ft_putstr_fd((char *)lst->data, 2);
			lst = lst->next;
		}
	}
	if (new_line)
		ft_putstr_fd("\n", 1);
	return (0);
}
