/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:42:33 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 12:43:52 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipex_error_i(char *message, int i)
{
	if (i != -1)
		ft_printf("%s at index %d\n", message, i);
	else
		ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}
//--FAIRE PLUTOT DES RETURN(NULL) POUR LAISSER LE PROG SE DEROULER ET FERMER
// TOUS LES FD ET ALLOC MEMOIRE

void	ft_free_tab(char **tab)
{
	int	i;

	if (tab == NULL)
		return ;
	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

int	pipex_creat_open_fd_in_one(char **argv)
{
	int	fd_in;

	fd_in = open(argv[1], O_RDONLY, 0644);
	if (fd_in == -1)
		pipex_error_i("open fd_in failed(pipex_creat_open_fd)", -1);
	return (fd_in);
}

int	pipex_creat_open_fd_out_one(int argc, char **argv)
{
	int	fd_out;

	fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
		pipex_error_i("open fd_out failed(pipex_creat_open_fd)", -1);
	return (fd_out);
}
