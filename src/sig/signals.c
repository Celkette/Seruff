/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:07:49 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/23 10:37:07 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
void	sig_ctrl_c_catcher(int n)
{
	(void)n;
	//char c;
	//char *term;
	//char buf[1000];
	//char *cm_cap = tgetstr("cm", NULL);
	//ft_memset(buf, 0, sizeof(char) * 1000);
	//term = getenv("TERM");
	//tgetent(buf, term);
	//printf("term = %s\n", term);
	//printf("buf = %s\n", buf);
	//printf("cm = %s\n", cm_cap);
	rl_on_new_line();
	//rl_replace_line("Minishell0-0$\n", 1);
	rl_replace_line("\n", 1);
	rl_redisplay();
	//tputs(tgoto(cm_cap, 0, 0), 1, putchar);
}
*/

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
