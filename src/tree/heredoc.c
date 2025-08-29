/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 07:10:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 00:23:48 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	end_heredoc(char *path, int i)
{
	char	*tmp;

	if (g_exit_code == 130)
		return ;
	tmp = ft_itoa(i);
	if (!tmp)
		return ;
	ft_putstr_fd("bash: warning: here-document at line ", 2);
	ft_putstr_fd(tmp, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd("')", 2);
	free(tmp);
	return ;
}

void	free_quit(t_env *minishell, t_r_in *redir, t_tree *node)
{
	char	*args[1];

	args[0] = NULL;
	close(redir->fd);
	unlink(redir->file);
	free(redir->file);
	free(redir);
	free(node);
	exit_minishell(minishell);
	execve("/", args, NULL);
}

void	while_read(t_env *minishell, t_r_in *redir)
{
	int		i;
	char	*s;

	i = 0;
	while (1)
	{
		i++;
		s = readline("> ");
		if (s)
		{
			if (ft_strncmp(s, redir->path, ft_strlen(redir->path)) \
	== 0 && ft_strlen(redir->path) == ft_strlen(s) && (free(s), 1))
				break ;
			s = fork_heredoc_expand(minishell, minishell->meta, redir, s);
			(write(redir->fd, s, ft_strlen(s)), write(redir->fd, "\n", 1));
			if (s)
				free(s);
		}
		else
		{
			end_heredoc(redir->path, i);
			break ;
		}
	}
}

void	fork_heredoc_n(t_env *minishell, t_r_in *redir, t_tree *node)
{
	if (!minishell || !redir)
		return ;
	close(redir->fd);
	redir->fd = open(redir->file, O_WRONLY, 0644);
	if (redir->fd < 0)
	{
		exit_minishell(minishell);
		exit(128);
	}
	signal(SIGINT, sigin_handler_heredoc);
	while_read(minishell, redir);
	free_quit(minishell, redir, node);
	exit(0);
}
