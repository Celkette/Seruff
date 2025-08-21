/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:34:14 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 11:58:05 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Recup le fd de la redirection out
// le token (="543>")
int	get_fd_redir_out(t_token *token)
{
	int	fd;

	if (!token || !token->data)
		return (-1);
	fd = ft_atoi(token->data);
	return (fd);
}

void	check_token(t_token *token, t_r_out *redir_out, int *i)
{
	if (((char *)token->data)[0] >= '0' && ((char *)token->data)[0] <= '9')
	{
		redir_out->fd = get_fd_redir_out(token);
		while (((char *)token->data)[*i] >= '0' && \
			((char *)token->data)[*i] <= '9')
			(*i)++;
	}
}
