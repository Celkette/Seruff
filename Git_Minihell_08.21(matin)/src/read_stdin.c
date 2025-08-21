/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:50:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/20 11:41:57 by cle-rouz         ###   ########.fr       */
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

/*
	* Copie de putchar glibc pour tputs
	* https://www.man7.org/linux/man-pages/man3/putchar.3p.html
*/
/*
 * Dans le cas ctrl-c premet de retirer les retour a la ligne
*/
//@Supprime tous les retours à la ligne (\n) dans une chaîne
//(Utilité : Nettoie la ligne lue par readline 
//pour éviter les problèmes avec les commandes)
// @param s La chaîne de caractères à modifier
/*
 * FONCTION TMP POUR TEST CD
*/
//@Teste si la commande est cd et l’exécute
//(Utilité : Fonction temporaire pour tester le changement de répertoire.)
// @param minishell Pointeur vers la structure d'environnement du shell
// @param str La chaîne de caractères contenant la commande
/*void	try_cd(t_env *minishell, char *str)
{
	char	**split;

	split = ft_split(str, ' ');
	if (!split)
		return ;
	if (ft_strncmp(split[0], "cd", 2) != 0)
	{
		free_split(split);
		return ;
	}
	cd(minishell, split[1]);
	free_split(split);
}*/

/*
	* Check la chaine du resultat de readline
	* clear = bash clear
	* exit	= bash exit
	*
	* pour tputs/tgetstr
* https://zestedesavoir.com/tutoriels/1733/termcap-et-terminfo/#4-tget-et-tputs
*/
//@Vérifie si la commande entrée est un built-in (exit, clear, pwd, env)
//@Ajoute la commande à l'historique si ce n'est pas un built-in
//(Utilité : Exécute le built-in ou ajoute la commande à l’historique.)
//@ Retourne -1 si la commande est "exit", 0 sinon
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
/*
static const char *token_type_print(t_token_type type)
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
            printf("key = [%s], val = [%s]\n", var->key ? var->key : "NULL", var->val ? var->val : "NULL");
        current = current->next;
        i++;
    }
}*/
void	try_tokenize_hell2(t_env *minishell, int valid)
{
	int	j;

	if (minishell && minishell->tree)
	{
		j = 0;
		if (valid && !g_readline.abort_exec)
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
	//print_token_list(minishell->token_list); //AJOUT DEBUG
	minishell->token_list = expand_the_list(minishell,
			minishell->token_list, meta);
	manage_var_decl(minishell);
	minishell->token_list = remove_list_quotes(minishell->token_list);
	//print_token_list(minishell->token_list); //AJOUT DEBUG
	if (!minishell->token_list)
		return ;
	if (check_redir(minishell->token_list) != 0)
		return ;
	valid = 1;
	g_readline.abort_exec = 0;
	minishell->tree = prepare(minishell, minishell->token_list, 0, &valid);
	g_readline.in_exec = 1;
	try_tokenize_hell2(minishell, valid);
}

//AVEC COMMENTAIRES
/*void	try_tokenize_hell(t_env *minishell, char *s, t_meta *meta)
{
	int		valid;

	free_token_list(minishell->token_list);
	minishell->token_list = tokenize_the_hell(s, "< >&|	");
	minishell->token_list = expand_the_list(minishell,
		minishell->token_list, meta);
	manage_var_decl(minishell);
	minishell->token_list = remove_list_quotes(minishell->token_list);
	if (!minishell->token_list)
		return ;
	if (check_redir(minishell->token_list) != 0)
		return ;
	/
	//t_list *current = minishell->token_list;
	//while (current)
	//{
	//	t_token *token = (t_token *)current->data;
	//	if (token)
	//	{
	//		printf("[try_tokenize_hell] Token: value=[%s], type=[%s],
		quote_=[%d]\n", token->data, token_type_print(token->type),
		token->quote_type); //AJOUT DEBUG
	//	}
	//	current = current->next;
	//}
	//print_var_list(minishell->env_list);
	
	valid = 1;
	g_readline.abort_exec = 0;
	minishell->tree = prepare(minishell, minishell->token_list, 0, &valid);
	//print_tree(minishell->tree, 0, ' ');
	g_readline.in_exec = 1;
	try_tokenize_hell2(minishell, valid);
}*/

//
/*static const char *token_type_print(t_token_type type)
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
void print_token_list(t_list *lst)
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

void print_var_list(t_list *list)
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
            printf("key = [%s], val = [%s]\n", var->key ? var->key : "NULL", var->val ? var->val : "NULL");
        current = current->next;
        i++;
    }
}*/

/*
	* DEFINE special prompt dans minishell_enum.h
	* read l'entre standart avec readline
	* Result -1 = quand la chaine str est "exit"
	* !str pour intercepter le EOF de ctrl + d
	*
	* Encore qql cas a gerer sur les sig ctrl-c ctrl-/
//@Affiche le type de token en fonction de son énumération
//(Utilité : Tester et visualiser le résultat du type de token
//@param type Le type de token à afficher
//@return Une chaîne de caractères représentant le type de token
//

const char *token_type_print(t_token_type type)
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
*/
