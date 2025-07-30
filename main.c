/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:28:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 13:29:12 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_env	minishell;
	t_meta	meta;
	t_list	*varlist;

	(void)ac;
	(void)av;
	if (env)
		minishell.env = env;
	varlist = NULL;
	ft_memset(&env, 0, sizeof(t_env));
	minishell.old_path = NULL;
	init_env(&minishell);
	init_sig(&minishell.sig);
	minishell.exec.is_pipeline = 0;
	minishell.exec.fd[0] = -1;
	minishell.exec.fd[1] = -1;
	minishell.exec.fd2[0] = -1;
	minishell.exec.fd2[1] = -1;
	copy_env_var(&minishell);
	read_stdin(&minishell, &meta, varlist);
	return (0);
}
