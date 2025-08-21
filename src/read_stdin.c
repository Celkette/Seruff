/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:50:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 14:51:04 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

static void	reset_term(t_env *minishell)
{
	struct termios	term;

	if (!minishell || !minishell->term_cpy)
		return ;
	term = *minishell->term_cpy;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
	write(STDOUT_FILENO, "\e[?1000l\e[?1002l\e[?1006l", 24);
}

int	wait_exec(t_env *minishell)
{
	pid_t	pid[2];
	int		code[3];

	if (!minishell)
		return (-1);
	code[2] = 0;
	pid[0] = wait(&code[1]);
	pid[1] = 0;
	while (pid[0] > 0)
	{
		if (pid[1] == 0 || (WIFEXITED(code[1]) && pid[0] > pid[1]))
		{
			code[0] = WEXITSTATUS(code[1]);
			pid[1] = pid[0];
		}
		pid[0] = wait(&code[2]);
		if (code[0] == 15)
			code[2] = 1;
	}
	if (code[2])
		reset_term(minishell);
	minishell->exec.exit_code = pid[0];
	return (code[0]);
}

void	try_tokenize_hell2(t_env *minishell, int valid)
{
	int	j;

	if (minishell && minishell->tree)
	{
		j = 0;
		if (valid)
		{
			exec_tree(minishell, minishell->tree, NULL, 0, &j);
			minishell->exec.exit_code = wait_exec(minishell);
		}
		free_tree(minishell->tree);
		minishell->tree = NULL;
	}
}

//@Découpe la ligne de commande en tokens 
//selon les séparateurs donnés et affiche chaque token
//(Utilité : Tester et visualiser le résultat du split (tokenisation))
// @param s La chaîne de caractères à découper
void	try_tokenize_hell(t_env *minishell, char *s, t_meta *meta)
{
	int		valid;

	free_token_list(minishell->token_list);
	minishell->token_list = tokenize_the_hell(s, "< >|	");
	minishell->token_list = expand_the_list(minishell,
			minishell->token_list, meta);
	manage_var_decl(minishell);
	minishell->token_list = remove_list_quotes(minishell->token_list);
	if (!minishell->token_list)
		return ;
	if (check_redir(minishell->token_list) != 0)
		return ;
	valid = 1;
	minishell->tree = prepare(minishell, minishell->token_list, 0, &valid);
	try_tokenize_hell2(minishell, valid);
}

/*
static const char *token_type_print(t_tkty type)
{
	switch (type)
	{
		case TOK_PIPE:			return ("0 - TOK_PIPE");
		case TOK_REDIR_OUT:		return ("1 - REDIR_OUT");
		case TOK_REDIR_IN:		return ("2 - REDIR_IN");
		case TOK_REDIR_APPEND:	return ("3 - REDIR_APPEND");
		case TOK_REDIR_HEREDOC:	return ("4 - REDIR_HEREDOC");
		case TOK_DOLLAR:		return ("5 - TOK_DOLLAR");
		case TOK_AMPERSAND:		return ("6 - TOK_AMPERSAND");
		case TOK_PARA_OPEN:		return ("7 - PARA_OPEN");
		case TOK_PARA_CLOSE:	return ("8 - PARA_CLOSE");
		case TOK_WORD:			return ("9 - TOK_WORD");
		case TOK_AND:			return ("10 - AND");
		case TOK_OR:			return ("11 - OR");
		default:                return ("UNKNOWN_TOKEN");
	}
}
static void print_token_list(t_list *lst)
{
	t_list	*current;
	t_token	*token;

	current = lst;
	while (current)
	{
		token = (t_token *)current->data;
		if (token)
		{
			printf("\nvalue=[%s]", token->data);
			printf("	type [%s], quote_ [%d]", token_type_print(token->type),
				token->quote_type);
		}
		current = current->next;
	}
	printf("\n");
}
*/
/*static void print_var_list(t_list *list)
{
    t_list *current = list;
    int i = 0;
    if (!list)
    {
        printf("  Var list is empty.\n");
        return;
    }
    while (current)
    {
        t_var *var = (t_var *)current->data;
        printf("Var[%d]: ", i);
        if (!var)
            printf("(null var)\n");
        else
            printf("key = [%s], val = [%s]\n", var->key ? var->key : "NULL",
				var->val ? var->val : "NULL");
        current = current->next;
        i++;
    }
}*/
