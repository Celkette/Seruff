/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:54:20 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 15:29:37 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	open_file(char *file, int flag)
{
	int	fd;

	fd = open(file, flag, 0644);
	if (fd == -1)
		pipex_error_i("open here_doc_fd failed(in pipex_creat_here_doc)", -1);
	return (fd);
}

int	pipex_creat_open_fd_in(char **argv)
{
	int	fd_in;

	if (check_if_heredoc(argv[1]) == 0)
	{
		fd_in = pipex_creat_here_doc(argv[2]);
		if (fd_in == -1)
			pipex_error_i("creat_here_doc failed", -1);
	}
	if (check_if_heredoc(argv[1]) != 0)
	{
		fd_in = open(argv[1], O_RDONLY, 0644);
		if (fd_in == -1)
			pipex_error_i("open fd_in failed(pipex_creat_open_fd)", -1);
	}
	return (fd_in);
}

int	pipex_creat_open_fd_out(int argc, char **argv)
{
	int	fd_out;

	if (check_if_heredoc(argv[1]) == 0)
	{
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_out == -1)
			pipex_error_i("open fd_out failed(pipex_creat_open_fd)", -1);
	}
	if (check_if_heredoc(argv[1]) != 0)
	{
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
			pipex_error_i("open fd_out failed(pipex_creat_open_fd)", -1);
	}
	return (fd_out);
}

static void	annexe(int here_doc_fd, char *line)
{
	free(line);
	close(here_doc_fd);
}

int	pipex_creat_here_doc(char *lim)
{
	int		h_d_fd;
	char	*line;
	size_t	lim_len;
	size_t	line_len;

	line = NULL;
	lim_len = ft_strlen(lim);
	h_d_fd = open_file("HereD.txt", O_CREAT | O_TRUNC | O_RDWR);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (ft_strncmp(line, lim, lim_len) == 0)
		{
			line_len = ft_strlen(line);
			if (line_len == lim_len || (line_len == lim_len + 1
					&& line[lim_len] == '\n'))
				return (annexe(h_d_fd, line), open_file("HereD.txt", O_RDONLY));
		}
		write(h_d_fd, line, ft_strlen(line));
		free(line);
	}
	close(h_d_fd);
	return (open_file("HereD.txt", O_RDONLY));
}
