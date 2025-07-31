/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:28:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/31 10:07:54 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

int	is_var_in_env(t_env *minishell, char *var_name);
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
	minishell.tree = NULL;
	minishell.env_list = NULL;
	minishell.old_path = NULL;
	init_env(&minishell);
	init_sig(&minishell.sig);
	minishell.exec.is_pipeline = 0;
	minishell.exec.fd[0] = -1;
	minishell.exec.fd[1] = -1;
	minishell.exec.fd2[0] = -1;
	minishell.exec.fd2[1] = -1;
	copy_env_var(&minishell);
	printf("is_var = %i\n", is_var_in_env(&minishell, "PATHH"));
	read_stdin(&minishell, &meta, varlist);
	return (0);
}
