/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:05:52 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 12:06:36 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

int	wait_exec_print(t_env *minishell, int code2, int code0)
{
	if (!minishell)
		return (0);
	if (code2)
		reset_term(minishell);
	if (code0 == 11)
	{
		ft_putstr_fd("Segmentation fault	(core dumped)\n", 2);
		minishell->exec.exit_code = 11 + 128;
		return (139);
	}
	else if (code0 == 3)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		minishell->exec.exit_code = 3 + 128;
		return (131);
	}
	if (g_exit_code == 130)
		minishell->exec.exit_code = 130;
	else
		minishell->exec.exit_code = code_converter(code0);
	return (minishell->exec.exit_code);
}

int	wait_exec(t_env *minishell)
{
	pid_t	pid[2];
	int		code[3];

	if (!minishell)
		return (-1);
	code[0] = 0;
	code[1] = 0;
	code[2] = 0;
	pid[0] = wait(&code[1]);
	pid[1] = 0;
	while (pid[0] >= 0)
	{
		if (pid[1] == 0 || pid[0] > pid[1])
		{
			if (WIFEXITED(code[1]))
				code[0] = WEXITSTATUS(code[1]);
			else if (WIFSIGNALED(code[1]))
				code[0] = WTERMSIG(code[1]);
			pid[1] = pid[0];
		}
		pid[0] = wait(&code[1]);
		if (code[0] == 143)
			code[2] = 1;
	}
	return (wait_exec_print(minishell, code[2], code[0]));
}
