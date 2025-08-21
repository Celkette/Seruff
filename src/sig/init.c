/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:18:17 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 10:45:06 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

int	init_sig(t_sig_manage *sig_manage)
{
	if (!sig_manage)
		return (-1);
	ft_memset(sig_manage, 0, sizeof(t_sig_manage));
	sig_manage->s_sig_ctrl_c.sa_handler = (void *)sig_ctrl_c_catcher;
	rl_event_hook = sigint_hook;
	sigaction(SIGINT, &sig_manage->s_sig_ctrl_c, NULL);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
//AVEC COMMENTAIRE
/*int	init_sig(t_sig_manage *sig_manage)
{
	if (!sig_manage)
		return (-1);
	ft_memset(sig_manage, 0, sizeof(t_sig_manage));
	// ajout la fonction a executer dans la structure
	// sig_manage
	sig_manage->s_sig_ctrl_c.sa_handler = (void *)sig_ctrl_c_catcher;
	// transfere la gestion du signal SIGINT a la structure
	// sig_manage
	rl_event_hook = sigint_hook;
	sigaction(SIGINT, &sig_manage->s_sig_ctrl_c, NULL);
	// SIGQUIT est ignore
	signal(SIGQUIT, SIG_IGN);
	return (0);
}*/