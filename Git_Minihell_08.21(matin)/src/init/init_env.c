/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:10:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 15:19:11 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
 * Recupere le PATH et le stock dans la struct minishell
*/
static int	get_path_env(t_env *minishell)
{
	char	**split_path;
	char	**env;

	split_path = NULL;
	env = minishell->env;
	while (*minishell->env)
	{
		if (ft_strncmp(*minishell->env, "PATH=", 5) == 0)
		{
			split_path = ft_split(*minishell->env + 5, ':');
			if (!split_path)
			{
				minishell->env = env;
				return (-1);
			}
			break ;
		}
		minishell->env++;
	}
	if (split_path)
		minishell->path_fun_split = split_path;
	else
		minishell->path_fun_split = NULL;
	minishell->env = env;
	return (0);
}

/*************************************
 * Import le pid de minishell
 * va dans /proc/self/status
 * et cherche la ligne qui commence
 * par "Pid:" avance jusquau premier
 * chiffre puis copie le reste de la
 * chaine dans minishell->pid
**************************************/
static void	get_minishell_pid(t_env *minishell)
{
	char	*tmp;
	char	*buf;
	int		fd;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd < 0)
		return ;
	tmp = get_next_line(fd);
	while (tmp)
	{
		if (ft_strstr(tmp, "Pid:"))
		{
			buf = tmp;
			while (*buf && *buf < '0' && *buf > '9')
				buf++;
			minishell->pid = ft_strndup(buf, ft_strlen(buf));
			break ;
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
	if (tmp)
		free(tmp);
	close(fd);
}

/*
* Charge lemplacement actuelle dans minishell->old_path
*/

static void	init_work_path(t_env *minishell)
{
	char	*tmp;
	size_t	size;

	size = 10;
	tmp = malloc(sizeof(char) * (size + 1));
	if (!tmp)
		return ;
	while (!getcwd(tmp, size))
	{
		free(tmp);
		if (errno == ENOENT)
		{
			ft_putstr_fd(ERROR_ENV, 2);
			return ;
		}
		if (errno != ERANGE)
			return ;
		size += 10;
		tmp = malloc(sizeof(char) * (size + 1));
		if (!tmp)
			return ;
	}
	minishell->act_path = tmp;
}

/*
 * Meme probleme que plus haut
 * Si il manque des info dans
 * l'env continuer lexec ou
 * arreter ?
*/
void	init_env(t_env *minishell)
{
	if (!minishell)
		return ;
	minishell->user.user = getenv("USER");
	minishell->user.home = getenv("HOME");
	minishell->user.uid = 0;
	init_work_path(minishell);
	load_env_user(minishell);
	get_minishell_pid(minishell);
	get_path_env(minishell);
	copy_env_var(minishell);
}
