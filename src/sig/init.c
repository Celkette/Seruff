/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:18:17 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 21:48:42 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <signal.h>

/*
	* init_sig permet dintialisier linterception des signaux
	* on recuperer la structure sigacton dans sig_manage
	* on lui donne n handler sig_ctrl_c_catcher
	* sig_ctrl_c_catcher seras executer grace a la
	* fonction sigaction lorsque le signal ctrl + c
	*
	* pour ctrl - \ on ignore grace a SIG_IGN
	*
	* Listes des signaux pour minishell
	*	SIGINT pour ctrl + c
	*	SIGQUIT pour ctrl + /
	*
	* sigaction http://manpagesfr.free.fr/man/man2/sigaction.2.html
*/

void	sig_kill_catcher(int n);

int	init_sig(t_sig_manage *sig_manage)
{
	if (!sig_manage)
		return (-1);
	ft_memset(sig_manage, 0, sizeof(t_sig_manage));
	sig_manage->s_sig_ctrl_c.sa_handler = (void *)sig_ctrl_c_catcher;
	sigaction(SIGINT, &sig_manage->s_sig_ctrl_c, NULL);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
