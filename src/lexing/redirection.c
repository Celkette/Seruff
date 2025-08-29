/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:54:11 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:51:57 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	check_in_1_next(t_list *lst, int fd)
{
	if (!lst)
		return (-1);
	fd = open(((t_token *)lst->data)->data, O_RDONLY, 0777);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(((t_token *)lst->data)->data, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	close(fd);
	return (0);
}

static void	print1_check_in_1(t_list *lst)
{
	ft_putstr_fd(ERROR_TOKEN2, 1);
	ft_putstr_fd(((t_token *)lst->data)->data, 1);
	ft_putstr_fd("'\n", 1);
}

static void	print2_check_in_1(t_list *lst)
{
	ft_putstr_fd(ERROR_TOKEN2, 2);
	ft_putstr_fd(((t_token *)lst->next->data)->data, 2);
	ft_putstr_fd("'\n", 2);
}

static void	print3_check_in_1(t_list *lst)
{
	ft_putstr_fd(ERROR_TOKEN2, 2);
	ft_putstr_fd(((t_token *)lst->data)->data, 2);
	ft_putstr_fd("'\n", 2);
}

int	check_in_1(t_list *lst)
{
	int	fd;

	if (!lst)
		return (-1);
	fd = -1;
	if (ft_strlen(((t_token *)lst->data)->data) == 1 && \
		((t_token *)lst->data)->data[0] == '<' && lst->next && \
		((t_token *)lst->next->data)->data[0] == '>')
	{
		return (print2_check_in_1(lst), 1);
	}
	if (ft_strlen(((t_token *)lst->data)->data) == 2 && \
		((t_token *)lst->data)->data[0] == '<' && lst->next && \
		((t_token *)lst->data)->data[1] == '<')
	{
		return (0);
	}
	if (lst->next && !is_an_redir(((t_token *)lst->next->data)->data[0]))
		return (check_in_1_next(lst->next, fd));
	else
		return (print3_check_in_1(lst), -1);
	print1_check_in_1(lst);
	return (1);
}
