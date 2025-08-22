/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:34:20 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:59:49 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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

static void	get_redir_in_here_doc2(t_r_in *redir_in, \
	char *buf, char *buf2, char *s)
{
	buf2 = ft_strndup(buf, ft_strlen(buf));
	if (buf)
		free(buf);
	buf = ft_strjoin(buf2, s);
	free(s);
	if (buf2)
		free(buf2);
	buf2 = ft_strjoin(buf, "\n");
	free(buf);
	buf = ft_strjoin(redir_in->path, buf2);
	free(buf2);
	if (redir_in->path)
		free(redir_in->path);
	redir_in->path = ft_strndup(buf, ft_strlen(buf));
	free(buf);
}

void	get_redir_in_here_doc(t_list *head, t_r_in *redir_in)
{
	char	*s;
	char	*buf;
	char	*buf2;
	t_token	*token;

	token = consume_token(head);
	g_readline.hd_active = 1;
	while (g_readline.hd_active)
	{
		s = readline("> ");
		if (s)
		{
			if (ft_strncmp(s, (char *)token->data, \
			ft_strlen((char *)token->data)) == 0 && \
			ft_strlen((char *)token->data) == ft_strlen(s))
			{
				free(s);
				break ;
			}
			buf = NULL;
			buf2 = NULL;
			get_redir_in_here_doc2(redir_in, buf, buf2, s);
		}
	}
}
