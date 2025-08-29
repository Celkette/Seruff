/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:37:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 01:19:00 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_an_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	print_error(char *data)
{
	ft_putstr_fd("minishell: ", 1);
	ft_putstr_fd(data, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(strerror(errno), 1);
}

int	open_redir(char *data, int not_append)
{
	int	fd;

	if (not_append)
	{
		fd = open(data, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (fd < 0)
			return (print_error(data), -1);
		close(fd);
	}
	else
	{
		fd = open(data, O_CREAT | O_WRONLY | O_APPEND, 0777);
		if (fd < 0)
			return (print_error(data), -1);
		close(fd);
	}
	return (0);
}

int	redir_out_append(char *data, t_list *lst)
{
	int	code;

	if (data && data[0] == '>' && data[1] != '>')
	{
		code = check_out_1(lst, 1);
		if (code != 0)
			return (2);
	}
	else if (data && data[0] == '>')
	{
		code = check_out_1(lst, 0);
		if (code != 0)
			return (2);
	}
	return (0);
}
