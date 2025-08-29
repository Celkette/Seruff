/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:07:49 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 21:48:52 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <readline/readline.h>

void	sig_ctrl_c_catcher(int n)
{
	(void)n;
	write(STDOUT_FILENO, "\n", 1);
	if (g_exit_code >= INT_MAX - 100)
		g_exit_code = g_exit_code - 200000;
	g_exit_code = 130;
	rl_on_new_line();
	rl_replace_line("", 1);
	if (g_exit_code == 130 || g_exit_code == 0)
		rl_redisplay();
	return ;
}

void	sig_ctrl_c_catcher2(int n)
{
	(void)n;
	write(STDOUT_FILENO, "\n", 1);
	if (g_exit_code >= INT_MAX - 100)
		g_exit_code = g_exit_code - 200000;
	g_exit_code = 130;
	rl_replace_line("", 1);
	rl_on_new_line();
	return ;
}

void	sigin_handler_heredoc(int n)
{
	(void)n;
	g_exit_code = 130;
	rl_replace_line("", 1);
	rl_on_new_line();
	close(STDIN_FILENO);
}

void	sig_kill_catcher(int n)
{
	(void)n;
}
