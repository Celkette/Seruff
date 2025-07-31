/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:07:49 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 08:11:16 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	sig_ctrl_c_catcher(int n)
{
	(void)n;
	// Change la valeur du buf de readline a vide
	rl_replace_line("", 0);
	// actualise la ligne
	rl_on_new_line();
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
	return ;
}
