/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 19:35:57 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 11:14:25 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	cd_exec(t_env *minishell, const char *path)
{
	DIR	*dir;

	dir = NULL;
	if (!minishell || !path)
		return (1);
	if (path[0] == '-' && (!path[1] || path[1] == ' ' || path[1] == '\t'))
		return (manage_back(minishell));
	if (path[0] == '~')
		return (manage_tile(minishell, path));
	if (path[0] == '-' && path[1])
		return (print_error(path));
	dir = prepare_cd(minishell, path, dir);
	if (!dir)
		return (1);
	if (chdir(path) != 0)
	{
		closedir(dir);
		chdir_error((char *)path, strerror(errno));
		return (1);
	}
	closedir(dir);
	return (0);
}


/*
	* Print les erreur de cd avec strerror et errno
*/
int	err_cd(const char *path)
{
	char	*error;

	if (!path)
		return (1);
	error = strerror(errno);
	ft_putstr_fd("minishell: cd: ", 1);
	ft_putstr_fd((char *)path, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(error, 1);
	return (1);
}

int	manage_tile(t_env *minishell, const char *path)
{
	char	*str;
	int		total_len;

	(void)str;
	(void)total_len;
	if (!minishell->user.home)
	{
		if (ft_strlen((char *)path) == 1)
		{
			printf("\n");
			return (1);
		}
		*(char *)path = ' ';
		return (cd_exec(minishell, path));
	}
	total_len = ft_strlen(minishell->user.home) + ft_strlen((char *)path);
	str = malloc(sizeof(char) * (total_len) + 1);
	ft_memcpy(str, minishell->user.home, \
		   sizeof(char) * (ft_strlen(minishell->user.home)));
	ft_memcpy(&str[ft_strlen(minishell->user.home)], &path[1], \
		   sizeof(char) * (ft_strlen((char *)path)));
	*(str + total_len) = 0;
	if (!str)
		return (0);
	return (manage_tile2(minishell, str, (char *)path));
}

int	print_error(const char *path)
{
	printf("minishell: cd: -%c: invalid option\n", path[1]);
	return (1);
}
//@ Permet de changer de repertoire de travail
// @param t_env *minishell, pointeur vers structure minishell
// @param const char *path repertoire voulu
// @return 0 si aucun probleme sinon return > 0
int	cd(void *p)
{
	t_arg	*arg;
	int		code;

	if (!p)
		return (0);
	arg = (t_arg *)p;
	if (!arg->node->arg)
	{
		free(p);
		return (0);
	}
	if (ft_lstsize(arg->node->arg) != 1)
	{
		free(p);
		return (0);
	}
	code = cd_exec(arg->minishell, (const char *)arg->node->arg->data);
	free(p);
	return (code);
}
