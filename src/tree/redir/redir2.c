/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:53:05 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 00:46:04 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Creer une redirection in  a partir dune liste de tokens
// avant dun token pour aller au fichier, puis le stock
// puis retourne la redir in
static t_r_in	*init_redir_in(t_token *token)
{
	t_r_in	*redir_in;

	redir_in = malloc(sizeof(t_r_in));
	if (!redir_in)
		return (NULL);
	ft_memset(redir_in, 0, sizeof(t_r_in));
	redir_in->type = ft_strlen(token->data);
	return (redir_in);
}

static int	handle_single_redir(t_list *head, t_r_in *redir_in)
{
	t_token	*token;

	token = consume_token(head);
	if (!token)
		return (-1);
	if (token->type != TOK_WORD)
		return (-1);
	redir_in->path = ft_strndup((char *)token->data, \
		ft_strlen((char *)token->data));
	return (0);
}

t_r_in	*get_redir_in(t_env *minishell, t_list *head, t_tree *node)
{
	t_r_in		*redir_in;
	t_token		*token;

	if (!head)
		return (NULL);
	token = consume_token(head);
	redir_in = init_redir_in(token);
	if (!redir_in)
		return (NULL);
	redir_in->is_quote = token->is_quote;
	if (redir_in->type == 1)
	{
		if (handle_single_redir(head, redir_in) != 0)
			return (free(redir_in), NULL);
	}
	else if (redir_in->type == 2)
	{
		g_exit_code = 0;
		token = consume_token(head);
		return (do_heredoc(redir_in, token, minishell, node));
	}
	else
		return (NULL);
	return (redir_in);
}
