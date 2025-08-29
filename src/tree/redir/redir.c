/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:34:20 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 23:16:17 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include <unistd.h>

// Creer une redirection out a partir dune liste de tokens
// set le fd de la redirection a -1 pour 
// dire que le fd nest pas donne dans le redir
// puis sil y a un fd dans le token on utilise 
// la fonction get_fd_redir_out pour le recup
// puis on avance dun token pour recup le 
// fichier dans le quelle ecrire
// puis retourne la redir out
t_r_out	*get_redir_out(t_list *head)
{
	t_r_out		*redir_out;
	t_token		*token;
	int			i;

	if (!head)
		return (NULL);
	redir_out = malloc(sizeof(t_r_out));
	if (!redir_out)
		return (NULL);
	ft_memset(redir_out, 0, sizeof(t_r_out));
	redir_out->fd = -1;
	token = consume_token(head);
	i = 0;
	check_token(token, redir_out, &i);
	redir_out->type = ft_strlen(&((char *)token->data)[i]);
	token = consume_token(head);
	if (!token)
	{
		free(redir_out);
		return (NULL);
	}
	redir_out->path = ft_strndup((char *)token->data, \
		ft_strlen((char *)token->data));
	return (redir_out);
}

char	*fork_heredoc_expand(t_env *minishell, \
	t_meta *meta, t_r_in *redir, char *s)
{
	char	*new_str;

	if (!minishell || !meta || !redir || !s || !*s)
		return (NULL);
	if (redir->is_quote == 0) //TODO Seth
	{
		new_str = expand(minishell, s, meta);
		free(s);
	}
	else
		new_str = s;
	return (new_str);
}
