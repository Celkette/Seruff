/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:50:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 10:08:12 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
//ft_putchar : affiche un caractère (pour tputs).
//remove_nl : enlève les retours à la ligne d’une chaîne.
//try_cd : exécute cd si demandé.
//check_str : gère les commandes spéciales et l’historique.
//try_split_hell : affiche le résultat du split de la ligne.
//read_stdin : boucle principale de lecture et traitement des commandes du shell.

/*
	* Copie de putchar glibc pour tputs
	* https://www.man7.org/linux/man-pages/man3/putchar.3p.html
*/
//@Affiche un caractère sur la sortie standard
//(Utilité : Sert de callback pour tputs (gestion du terminal))
// Retourne le caractère affiché ou EOF en cas d'erreur.
static int	ft_putchar(int c)
{
	if (write(1, &c, 1) != 1)
		return (EOF);
	return (c);
}

int clearr(void *p)
{
	char	*clap;

	(void)p;
	clap = tgetstr("cl", NULL);
	tputs(clap, 1, ft_putchar);
	return (0);
}
/*
 * Dans le cas ctrl-c premet de retirer les retour a la ligne
*/
//@Supprime tous les retours à la ligne (\n) dans une chaîne
//(Utilité : Nettoie la ligne lue par readline pour éviter les problèmes avec les commandes)
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

/*
	* DEFINE special prompt dans minishell_enum.h
	* read l'entre standart avec readline
	* Result -1 = quand la chaine str est "exit"
	* !str pour intercepter le EOF de ctrl + d
	*
	* Encore qql cas a gerer sur les sig ctrl-c ctrl-/
*/
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

int	wait_exec(void)
{
	pid_t	pid[2];
	int		status;
	int		end;

	pid[0] = wait(&status);
	pid[1] = 0;
	while (pid[0] > 0)
	{
		if (pid[1] == 0 || (WIFEXITED(status) && pid[0] > pid[1]))
		{
			end = WEXITSTATUS(status);
			pid[1] = pid[0];
		}
		pid[0] = wait(&status);
	}
	return (end);
}

void	print_token_list(t_list *lst)
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
			printf("	type [%s], quote_ [%d]\n", token_type_print(token->type), token->quote_type);
		}
		current = current->next;
	}
}

//@Découpe la ligne de commande en tokens selon les séparateurs donnés et affiche chaque token
//(Utilité : Tester et visualiser le résultat du split (tokenisation))
// @param s La chaîne de caractères à découper
void	try_tokenize_hell(t_env *minishell, char *s, t_list *var_list, t_meta *meta)
{
	t_token	*token;
	t_list	*token_list;
	t_list	*current;
	int		valid;
	int		j;
//	int		has_cmd;

	token_list = tokenize_the_hell(s, "<>&| ");
	//print_token_list(token_list);
	token_list = expand_the_list(minishell, token_list, meta, var_list);
	//print_token_list(token_list);
	token_list = remove_list_quotes(token_list);
	//print_token_list(token_list);
	if (!token_list)
		return ;
	if (check_redir(token_list) != 0)
	{
		free_token_list(token_list);
		return ;
	}
	current = token_list;
	while (current)
	{
		token = (t_token *)current->data;
		if (token)
		{
			//printf("\n===value=[%s]", token->data);
			//printf("===type [%s], quote_ [%d]\n", token_type_print(token->type), token->quote_type);
		}
		current = current->next;
	}
	valid = 1;
	minishell->tree = prepare(minishell, token_list, 0, &valid);
	//print_tree(minishell->tree, 0, 'R');
	if (minishell->tree)
	{
		j = 0;
		if (valid)
		{
			exec_tree(minishell, minishell->tree, NULL, 0, &j);
			minishell->exec.exit_code = wait_exec();
		}
		free_tree(minishell->tree);
		minishell->tree = NULL;
	}
	free_token_list(token_list);
}

//@ Boucle principale du shell
//@lit l’entrée utilisateur, traite les commandes spéciales, affiche le split, puis lance le lexing.
//(Utilité : Cœur de la boucle d’interprétation du shell.)
// @param minishell Pointeur vers la structure d'environnement du shell
void	read_stdin(t_env *minishell, t_meta *meta, t_list *varlist)
{
	char	*str;

	str = NULL;
	while (1)
	{
		if (SPECIAL_PROMPT)
			str = readline("\r>");
		else
			str = readline("\rminishell0-0$ ");
		if (!str)
		{
			free(str);
			rl_clear_history();
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		else if (str)
		{
			add_history(str);
			if (check_lexing(str, meta) == 0 && !ft_strchr(str, '&'))
				try_tokenize_hell(minishell, str, varlist, meta);
			//check_lexing(str);
			free(str);
			str = NULL;
		}
	}
	exit_minishell(minishell);
//	check_lexing(str, meta);// A SUPPRIMER
}
