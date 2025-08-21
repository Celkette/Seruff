/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 05:28:53 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/23 09:43:47 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*realloc_char(t_file *file, int linebreak)
{
	int		i;
	int		j;
	int		k;
	char	*new;

	k = ft_strlen_gnl(file->line, 0);
	new = ft_give_malloc(linebreak, k + \
					ft_strlen_gnl(&file->buf[file->index], 1));
	if (new == NULL)
		return (NULL);
	i = -1;
	j = file->index;
	while (++i < k && file->line != NULL)
		new[i] = file->line[i];
	while (j - file->index < ft_strlen_gnl(&file->buf[file->index], 1))
		new[i++] = file->buf[j++];
	file->index = j;
	if (file->line != NULL)
		free(file->line);
	file->line = new;
	if (linebreak)
		file->index++;
	return (file->line);
}

/*
void	debug(t_file *f)
{
	printf(" ---- | debug | ---- \n");
	printf("fd: %i\n", f->fd);
	printf("status: %i\n", f->status);
	printf("index: %i\n", f->index);
	printf("buf: %s\n", f->buf);
	if (f->line != NULL)
		printf("line: %s\n", f->line);
}
*/

void	*read_error(t_file *file, int freee)
{
	if (file->line && freee)
		free(file->line);
	clear_buffer(file);
	return (NULL);
}

char	*logic(t_file *file)
{
	char	*tmp;

	while (file->status > 0)
	{
		realloc_char(file, 0);
		if (file->index == BUFFER_SIZE)
		{
			clear_buffer(file);
			file->status = read(file->fd, file->buf, BUFFER_SIZE);
			if (file->status == -1)
				return (read_error(file, 1));
		}
		else if (file->buf[file->index] == '\n')
			return (realloc_char(file, 1));
		else if (file->buf[file->index] == '\0')
		{
			realloc_char(file, 0);
			tmp = file->line;
			if (ft_strlen_gnl(file->line, 0) == 0)
				return (read_error(file, 1));
			return (tmp);
		}
	}
	return (file->line);
}

char	*next_line(t_file *file)
{
	if (file->index == BUFFER_SIZE)
	{
		clear_buffer(file);
		file->status = read(file->fd, file->buf, BUFFER_SIZE);
		file->index = 0;
	}
	if (file->status == -1)
		return (read_error(file, 1));
	file->line = logic(file);
	return (file->line);
}

char	*get_next_line(int fd)
{
	static t_file		file[1025];
	char				*tmp;

	if (file[fd].init == 0)
		init_file2(&file[fd], fd);
	if (file[fd].line != NULL)
		file[fd].line = NULL;
	tmp = next_line(&file[fd]);
	if (tmp == NULL)
	{
		clear_file(&file[fd]);
		return (NULL);
	}
	return (tmp);
}
