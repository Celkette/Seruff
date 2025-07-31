/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:34:20 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 08:54:38 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// Recup le fd de la redirection out
// le token (="543>")
static int		get_fd_redir_out(t_token *token)
{
	int	fd;

	if (!token || !token->data)
		return (-1);
	fd = ft_atoi(token->data);
	return (fd);
}

// Creer une redirection out a partir dune liste de tokens
// set le fd de la redirection a -1 pour dire que le fd nest pas donne dans le redir
// puis sil y a un fd dans le token on utilise la fonction get_fd_redir_out pour le recup
// puis on avance dun token pour recup le fichier dans le quelle ecrire
// puis retourne la redir out
t_redir_out	*get_redir_out(t_list *head)
{
	t_redir_out	*redir_out;
	t_token		*token;
	int			i;

	if (!head)
		return (NULL);
	redir_out = malloc(sizeof(t_redir_out));
	if (!redir_out)
		return (NULL);
	ft_memset(redir_out, 0, sizeof(t_redir_out));
	redir_out->fd = -1;
	token = consume_token(head);
	i = 0;
	if (((char *)token->data)[0] >= '0' && ((char *)token->data)[0] <= '9')
	{
		redir_out->fd = get_fd_redir_out(token);
		while (((char *)token->data)[i] >= '0' && ((char *)token->data)[i] <= '9')
			i++;
	}
	redir_out->type = ft_strlen(&((char *)token->data)[i]);
	token = consume_token(head);
	if (!token)
	{
		free(redir_out);
		return (NULL);
	}
	redir_out->path = ft_strndup((char *)token->data, ft_strlen((char *)token->data));
	return (redir_out);
}

// Creer une redirection in  a partir dune liste de tokens
// avant dun token pour aller au fichier, puis le stock
// puis retourne la redir in
t_redir_in	*get_redir_in(t_list *head)
{
	t_redir_in	*redir_in;
	t_token		*token;

	if (!head)
		return (NULL);
	token = consume_token(head);
	redir_in = malloc(sizeof(t_redir_in));
	if (!redir_in)
		return (NULL);
	ft_memset(redir_in, 0, sizeof(t_redir_in));
	redir_in->type = ft_strlen(token->data);
	token = consume_token(head);
	if (!token)
	{
		free(redir_in);
		return (NULL);
	}
	if (token->type != TOK_WORD)
	{
		free(redir_in);
		return (NULL);
	}
	redir_in->path = ft_strndup((char *)token->data, ft_strlen((char *)token->data));
	return (redir_in);
}
