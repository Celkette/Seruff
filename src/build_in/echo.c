/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 05:23:52 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 11:19:11 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	need_to_print(char *str)
{
	if (!str)
		return (0);
	if (*(str++) != '-')
		return (1);
	while (*str)
	{
		if (*str != 'n')
			return (1);
		str++;
	}
	return (0);
}

static void	arg_echo(t_list *lst, int *new_line)
{
	if (!lst)
		return ;
	while (lst && !need_to_print(lst->data))
	{
		*new_line = 0;
		lst = lst->next;
	}
	while (lst)
	{
		ft_putstr_fd(lst->data, 1);
		if (lst->next)
			ft_putstr_fd(" ", 1);
		lst = lst->next;
	}
}

int	echoo(void *p)
{
	t_arg	*arg;
	int		new_line;

	new_line = 1;
	if (!p)
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	arg = (t_arg *)p;
	if (arg->node && arg->node->arg)
		arg_echo(arg->node->arg, &new_line);
	if (new_line)
		ft_putstr_fd("\n", 1);
	if (arg->node->top)
		exit(0);
	if (p)
		free(p);
	return (0);
}
