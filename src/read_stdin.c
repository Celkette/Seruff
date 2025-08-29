/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:50:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 19:54:06 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

void	reset_term(t_env *minishell)
{
	struct termios	term;

	if (!minishell || !minishell->term_cpy)
		return ;
	term = *minishell->term_cpy;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

int	code_converter(int code)
{
	if (code == 512)
		return (2);
	else if (code == 256)
		return (1);
	else if (code < 256)
		return (code);
	else
		return (code % 256);
}

void	update_before_exec(t_env *minishell, t_tree *tree)
{
	if (minishell->last_cmd)
		free(minishell->last_cmd);
	if (tree->built_in)
		update_last_cmd(minishell, ft_strndup(tree->content, \
		ft_strlen(tree->content)));
	else if (tree->content)
		update_last_cmd(minishell, ft_strndup(tree->content, \
		ft_strlen(tree->content)));
}

void	valid_tree(t_env *minishell, t_tree *tree, int valid)
{
	int	n;

	n = 0;
	if (valid && tree)
	{
		if (!minishell->tree->type)
		{
			update_before_exec(minishell, tree);
			minishell->exec.exit_code = exec_one_cmd(minishell, tree);
			if (tree && !tree->built_in)
				wait_exec(minishell);
		}
		else
		{
			exec_tree(minishell, minishell->tree, NULL, &n);
			minishell->exec.exit_code = wait_exec(minishell);
		}
	}
	if (g_exit_code == 130)
	{
		g_exit_code = 0;
		minishell->exec.exit_code = 130;
	}
}

int	check_redir_2(t_list *lst)
{
	t_token	*tok;
	int		i;

	i = 0;
	while (lst)
	{
		tok = (t_token *)lst->data;
		if (tok->redir)
			i++;
		else
			i = 0;
		if (i > 1)
			return (1);
		lst = lst->next;
	}
	return (0);
}
