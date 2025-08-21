/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:45:53 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/08 17:39:27 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	clear_buffer(t_file *file)
{
	int	i;

	i = 0;
	while (i <= BUFFER_SIZE)
		file->buf[i++] = '\0';
	file->index = 0;
}

void	clear_file(t_file *file)
{
	clear_buffer(file);
	file->fd = 0;
	file->init = 0;
	file->line = NULL;
	file->status = 0;
	file->index = 0;
}

void	init_file2(t_file *file, int fd)
{
	file->fd = fd;
	file->init = 1;
	file->line = NULL;
	clear_buffer(file);
	file->status = read(fd, file->buf, BUFFER_SIZE);
}

int	ft_strlen_gnl(char *s, int endl)
{
	int	i;

	if (s == NULL)
		return (0);
	i = 0;
	if (!endl)
		while (s[i])
			i++;
	else
		while (s[i] && s[i] != '\n')
			i++;
	return (i);
}

char	*ft_give_malloc(int linebreak, int size)
{
	char	*new;

	if (linebreak)
		new = malloc(sizeof(char) * (size + 2));
	else
		new = malloc(sizeof(char) * (size + 1));
	if (new == NULL)
		return (NULL);
	if (linebreak)
		new[size++] = '\n';
	new[size] = '\0';
	return (new);
}
