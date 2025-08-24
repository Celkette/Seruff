/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:28:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:32:21 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

volatile sig_atomic_t exit_code;

int	main(int ac, char **av, char **env)
{
	t_env			minishell;
	t_meta			meta;
	struct termios	test;
	t_list			*varlist;

	(void)ac;
	(void)av;
	(void)varlist;
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
	minishell.meta = &meta;
	read_stdin(&minishell, &meta, NULL, NULL);
	return (0);
}
