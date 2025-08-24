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

static char	*fork_heredoc_expand(t_env *minishell, \
	t_meta *meta, t_token *token, char *s)
{
	char	*new_str;

	if (!minishell || !meta || !token || !s || !*s)
		return (NULL);
	if (token->is_quote == 0)
	{
		new_str = expand(minishell, s, meta);
		free(s);
	}
	else
		new_str = s;
	return (new_str);
}

static void	fork_heredoc(t_env *minishell, t_r_in *redir_in, t_token *token)
{
	int		fd;
	char	*s;

	if (access(redir_in->path, W_OK) == 0)
		fd = open(redir_in->path, O_WRONLY, 0644);
	else
		fd = open(redir_in->path, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		(fprintf(stderr, "erro = %s\n", strerror(errno)), exit(0));
	signal(SIGINT, sigin_handler_heredoc);
	while (1)
	{
		s = readline("> ");
		if (s)
		{
			if (ft_strncmp(s, (char *)token->data, ft_strlen(token->data)) \
	== 0 && ft_strlen(token->data) == ft_strlen(s) && (free(s), 1))
				break ;
			s = fork_heredoc_expand(minishell, minishell->meta, token, s);
			(write(fd, s, ft_strlen(s)), write(fd, "\n", 1), free(s));
		}
	}
	(close(fd), free(redir_in->path), free(redir_in), \
		exit_minishell(minishell), exit(0));
}

char	*get_file_name(void)
{
	int		fd;
	char	*buf;

	buf = malloc(sizeof(char) * 20);
	if (!buf)
		return (NULL);
	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		free(buf);
		return (NULL);
	}
	if (read(fd, buf, 19) == -1)
	{
		free(buf);
		close(fd);
		return (NULL);
	}
	close(fd);
	buf[19] = 0;
	return (buf);
}

void	get_redir_in_here_doc(t_env *minishell, t_list *head, t_r_in *redir_in)
{
	pid_t	proc;
	t_token	*token;
	int		code;

	code = 0;
	token = consume_token(head);
	redir_in->path = get_file_name();
	proc = fork();
	if (proc < 0)
		return ;
	else if (proc == 0)
		fork_heredoc(minishell, redir_in, token);
	if (redir_in->fd < 0)
		return ;
	waitpid(proc, &code, 0);
	redir_in->fd = open(redir_in->path, O_RDONLY, 0644);
	unlink(redir_in->path);
}
