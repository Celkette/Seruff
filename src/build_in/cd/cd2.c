/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:34:28 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/14 01:41:41 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	manage_tile2(t_env *minishell, char *path, char *og)
{
	DIR	*dir;

	dir = NULL;
	dir = prepare_cd2(minishell, path, og, dir);
	if (!dir)
	{
		free(path);
		return (1);
	}
	if (chdir(path) != 0)
	{
		closedir(dir);
		free(path);
		chdir_error((char *)og, strerror(errno));
		return (1);
	}
	closedir(dir);
	free(path);
	return (0);
}

/*
	* open dir un "flux" vers le repertoire demande
	* si echoue pas de repertoire donc print erreur
	* stat recupere la struct stat avec les infos
	* du repertoire (pour checker les permissions)
	* puis verifie les droits avec check_perm
	* si OK chdir pour changer le repertoire de 
	* travail
	*
	* Doc:
	* http://manpagesfr.free.fr/man/man3/opendir.3.html
	* http://manpagesfr.free.fr/man/man2/stat.2.html
	* http://manpagesfr.free.fr/man/man2/chdir.2.html
*/

DIR	*prepare_cd(t_env *minishell, const char *path, DIR *dir)
{
	struct stat	dir_struct;

	if (!path)
		return (0);
	dir = opendir(path);
	if (!dir)
	{
		err_cd(path);
		return (0);
	}
	if (stat(path, &dir_struct) != 0)
	{
		closedir(dir);
		err_stat(path);
		return (0);
	}
	if (check_perm(minishell, &dir_struct) == 0)
	{
		closedir(dir);
		printf("minishell: cd: %s: Permission denied\n", (char *)path);
		return (NULL);
	}
	return (dir);
}

DIR	*prepare_cd2(t_env *minishell, const char *path, char *og, DIR *dir)
{
	struct stat	dir_struct;

	if (!path)
		return (0);
	dir = opendir(path);
	if (!dir)
	{
		err_cd(og);
		return (0);
	}
	if (stat(path, &dir_struct) != 0)
	{
		closedir(dir);
		err_stat(og);
		return (0);
	}
	if (check_perm(minishell, &dir_struct) == 0)
	{
		closedir(dir);
		printf("minishell: cd: %s: Permission denied\n", og);
		return (NULL);
	}
	return (dir);
}
