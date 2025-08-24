/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:07:49 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 10:45:57 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <readline/readline.h>

void	sig_ctrl_c_catcher(int n)
{
	(void)n;
	write(STDOUT_FILENO, "\n", 1);
	if (exit_code == -1)
	{
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		rl_on_new_line();
		rl_replace_line("", 1);
	}
	return ;
}

void	sigin_handler_heredoc(int n)
{
	(void)n;
	rl_replace_line("", 1);
	exit(130);
}

void	sig_kill_catcher(int n)
{
	(void)n;
}

//AVEC COMMENTAIRE
/*void	sig_ctrl_c_catcher(int n)
{
	(void)n;
	// Change la valeur du buf de readline a vide
	if (g_readline.hd_active)
	{
		g_readline.hd_active_sigint = 1;
		return ;
	}
	if (g_readline.in_exec)
		write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	// actualise la ligne
	rl_on_new_line();
	rl_done = 1;
	return ;
}*/
/*int	sigint_hook(void)
{
	//fprintf(stderr, "bonjour je suis un hook\n");
	if (g_readline.hd_active_sigint)
	{
		g_readline.hd_active_sigint = 0;
		g_readline.hd_active = 0;
		g_readline.abort_exec = 1;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
	return (0);
}*/
