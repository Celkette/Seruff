/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:29:51 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 20:09:01 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
	* utilise getcwd pour recuperer le repertoir de travail
	* si getcwd echoue avec le code ENOENT, le repertoire 
	* de travail a ete supprime donc on affiche lerreur
	* si EACCESS Impossible de lire ou de parcourir un 
	* composant du chemin d'accès, mais impossibe a 
	* provoquer ??????????
	*
	* doute sur lerreur recuperer depuis bash --posix 5.2
	* a verifier sur les pc 42
	*
	* si getcwd reussi ecris le repertoire de travail dans
	* buf puis on le print avec printf
	*
	* PS: les autres erreur de getcwd sont liee au buf
	*
	* Doc:
	* http://manpagesfr.free.fr/man/man3/getcwd.3.html
*/

int	error_pwd(void *p)
{
	ft_putstr_fd("pwd: error retrieving current directory: getcwd: \
	cannot access parent directories: No such file or directory\n", 2);
	if (p)
	{
		if (((t_arg *)p)->node->top)
			exit_minishell(((t_arg *)p)->minishell);
		free(p);
	}
	return (ENOENT);
}

int	pwd(void *p)
{
	char	buf[102400];

	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		if (use_redir_out(((t_arg *)p)->node) != 0)
			return (1);
	if (!getcwd(buf, 102400))
	{
		if (errno == ENOENT)
			return (error_pwd(p));
	}
	ft_putendl_fd(buf, 1);
	if (!((t_arg *)p)->node->top && ((t_arg *)p)->node->stdout_cpy != 0)
		dup_close(((t_arg *)p)->node->status, STDOUT_FILENO);
	if (!((t_arg *)p)->node->top && ((t_arg *)p)->node->stdin_cpy != 0)
		dup_close(((t_arg *)p)->node->status, STDIN_FILENO);
	if (((t_arg *)p)->node->top)
		exit_minishell(((t_arg *)p)->minishell);
	if (p)
		free(p);
	return (0);
}
