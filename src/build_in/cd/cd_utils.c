/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:36:02 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 11:59:24 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	err_stat(const char *path)
{
	char	*error;

	if (!path)
		return (1);
	error = strerror(errno);
	ft_putstr_fd("minishell: stat: ", 1);
	ft_putstr_fd((char *)path, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(error, 1);
	return (1);
}

/*
	* verifie les permission en fonction de luser
	* utilise in mask define par gnu return 0
	* si pas le droit ou un int > 0
	*
	* Doc:
	*https://www.gnu.org/software/libc/manual/2.29/html_node/Permission-Bits.html
	*https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
*/

int	check_perm(t_env *minishell, struct stat *dir)
{
	if (minishell->user.uid == dir->st_uid)
		return (dir->st_mode & S_IXUSR);
	else if (minishell->user.gid == dir->st_gid)
		return (dir->st_mode & S_IXGRP);
	return (dir->st_mode & S_IXOTH);
}

void	chdir_error(char *path, char *err)
{
	if (!path || !err)
		return ;
	ft_putstr_fd("minishell: chdir: ", 1);
	ft_putstr_fd(path, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(err, 1);
}

int	set_new_old_path(t_env *minishell, const char *path)
{
	minishell->old_path = ft_strndup(path, ft_strlen((char *)path));
	printf("old path = %s\n", minishell->old_path);
	if (!minishell->old_path)
		return (1);
	return (0);
}

int	manage_back(t_env *minishell)
{
	if (!minishell->old_path)
	{
		printf("%s\n", minishell->act_path);
		return (0);
	}
	return (cd_exec(minishell, minishell->old_path));
}
