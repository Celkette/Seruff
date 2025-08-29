/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:10:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 20:40:22 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

static void	check_required_var(t_env *minishell)
{
	char	str[100000];
	t_var	*tmp;

	if (!is_var_in_env(minishell, "PWD"))
	{
		if (getcwd(str, 99999))
		{
			tmp = create_classic_var("PWD", str);
			if (tmp)
				ft_lstadd_back(&minishell->env_list, ft_create_node(tmp));
		}
	}
	if (!is_var_in_env(minishell, "SHLVL"))
	{
		ft_lstadd_back(&minishell->env_list, \
		ft_create_node(create_classic_var("SHLVL", "1")));
	}
}

void	init_spe_var(t_env *minishell)
{
	init_work_path(minishell);
	load_env_user(minishell);
	get_minishell_pid(minishell);
	get_path_env(minishell);
	copy_env_var(minishell);
}

/*
 * Meme probleme que plus haut
 * Si il manque des info dans
 * l'env continuer lexec ou
 * arreter ?
*/
void	init_env(t_env *minishell)
{
	t_var	*shlvl;
	char	*tmp;
	int		lvl;

	if (!minishell)
		return ;
	init_spe_var(minishell);
	minishell->user.user = getenv("USER");
	minishell->user.home = getenv("HOME");
	shlvl = get_env_var(minishell, "SHLVL");
	if (shlvl)
	{
		lvl = ft_atoi(shlvl->val);
		if (lvl >= 0 && lvl <= INT_MAX - 10)
			lvl++;
		else
			lvl = 0;
		tmp = ft_itoa(lvl);
		if (!tmp)
			return (check_required_var(minishell));
		free(shlvl->val);
		shlvl->val = tmp;
	}
	check_required_var(minishell);
}
