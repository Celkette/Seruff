/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:28:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/15 15:29:39 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"
#include "src/headers/minishell_fun.h"

t_manage_readline	g_readline;

int	main(int ac, char **av, char **env)
{
	t_env	minishell;
	t_meta	meta;
	struct termios	test;
	t_list	*varlist;

	(void)ac;
	(void)av;
	(void)varlist;
	g_readline.hd_active = 0;
	g_readline.hd_active_sigint = 0;
	g_readline.in_exec = 0;
	g_readline.abort_exec = 0;
	ft_memset(&minishell, 0, sizeof(t_env));
	if (env)
		minishell.env = env;
	varlist = NULL;
	init_env(&minishell);
	init_sig(&minishell.sig);
	minishell.exec.is_pipeline = 0;
	minishell.exec.fd[0] = -1;
	minishell.exec.fd[1] = -1;
	minishell.exec.fd2[0] = -1;
	minishell.exec.fd2[1] = -1;
	ft_memset(&test, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &test);
	minishell.term_cpy = &test;
	read_stdin(&minishell, &meta);
	return (0);
}
