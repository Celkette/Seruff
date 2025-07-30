/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_the_hell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:42:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/30 12:13:11 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// EX split_the_hell

/***********************************************************
*      decoupe depuis la chaine et renvoie le token        *
* si hors quote supprime les espaces(debut et fin de str)  *
************************************************************/
//@ Alloue & copie une sous-chaîne de str de longueur n, en retirant les espaces au début et à la fin.
// @param str La chaîne d'origine à dupliquer
// @param n La longueur maximale de la sous-chaîne à dupliquer
// @return Un pointeur vers la nouvelle chaîne dupliquée, ou NULL en cas d'erreur
char *ft_strndup_token(const char *str, int n)
{//ex  *ft_strndup_split
	char	*tmp;
	int		i;
	int		len;
	
	len = n -1;
	i = 0;
	if (str[i] != '\'' && str[i] != '\"')
	{
		while (str[i] && i < n && (str[i] == ' ' || str[i] == '\t'))// Ignore les espaces du début de str
			i++;
		while (len > 0 && str[len] && (str[len] == ' ' || str[len] == '\t'))// Ignore les espaces de la fin de str
			len--;
	}
	// Vérifie si str commence et termine par une quote
 	//et supprime quote ouvrante et fermante
	/*if ((str[i] == '\'' && ft_str[len] == '\'') || (str[i] == '\"' && ft_str[len] == '\"')) 
	{
    	i++;    					// Ignore le premier caractère (quote ouvrante)
        len--;  					// Ignore le dernier caractère (quote fermante)
		if (len - i + 1 == 0)		// Si la longueur de la sous-chaîne est 0
			return (ft_strdup(""));	// retourne une chaîne vide
	}*/
	if (len - i + 1 <= 0)// Si la longueur de la sous-chaîne est inférieure ou égale à 0, retourne NULL
		return (NULL);
	tmp = malloc(sizeof(char) * (len - i + 2));// +1 pour le caractère nul
	if (!tmp)
	{
		perror("Memory allocation error for ft_strndup_token");
		return (NULL);
	}
	tmp = ft_memcpy(tmp, &str[i], sizeof(char) * (len - i + 1));// Copie la sous-chaîne depuis str[i] jusqu'à len
	tmp[len - i + 1] = 0;// Termine la nouvelle chaîne par un caractère nul
		
	return (tmp);
}
/***************************************
*   verifie si char est un separateur  *
****************************************/
//@Vérifie si le caractère c appartient à la chaîne charset.
// @param c Le caractère à vérifier
// @param charset La chaîne de caractères dans laquelle vérifier
// @return 1 si c est dans charset, 0 sinon
int is_in_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}
/***********************************
*   verifie si char est une quote  *
************************************/
//@Vérifie si le caractère c est une quote (guillemet simple ou double).
// @param Pour eviter de modifier les flag dans la structure meta
// @return 1 si c est une quote, 0 sinon
/*static	int is_it_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}*/

//@Vérifie si la chaîne est composée de chiffres uniquement.
// @param s La chaîne de caractères à vérifier
// @param limit La longueur maximale à vérifier dans la chaîne
// @return 1 si la chaîne est un nombre entier positif, 0 sinon
/*int	is_only_digit(char *s, int limit)
{
	int	i;

	if (!s || limit < 0)
		return (0);
	i = 0;
	while (i <= limit && s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}*/

int	get_charset_length(char *s, char *charset)
{
	char	c;
	int		i;

	if (!s || is_in_charset(s[0], charset) == 0)
		return (0);
	c = s[0];
	i = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i] && (s[i] == '\t' || s[i] == ' '))
		i++;
	return (i);
}

/*********************************************
*   compte le nombre de char du futur token  *
**********************************************/
//@Calculer la longueur d’un bloc homogène : une suite de caractères qui sont tous soit dans quote, soit tous hors quote
// @param s La chaîne de caractères à analyser
// @param charset La chaîne de caractères définissant le charset
// @return La longueur du bloc homogène

size_t	ft_token_len(char *s, char *charset, t_meta *meta)
{
	size_t	j;
	char quote_char;

	j = 0;
	if (is_in_charset(s[j], "0123456789"))
    {
        while (is_in_charset(s[j], "0123456789"))
            j++;
        if (s[j + 1] && ((s[j]  == '>' && s[j + 1]  == '>') || (s[j]  == '<' && s[j + 1]  == '<')))
			return (j + 2); // Cas spécial pour les redirections numériques 
		if (s[j]  == '>' || s[j]  == '<')
			return (j + 1);
	}
	j = 0;
	while (s[j])
	{
		meta->last_quote_is = s[j];
		if (is_in_charset(s[j], charset)) // Si on rencontre un séparateur (charset), on s'arrête
		{	// Cas spéciaux pour les opérateurs multi-caractères
			if (j > 0) //si on a déjà accumulé des caractères avant le séparateur
				return (j);
			if (s[j] && s[j + 1] && s[j + 2] && s[j] == '<' && s[j + 1] == '<'
				&& s[j + 2] == '<')//si <<<
				return (3); // Cas spécifique pour cree token [<<<]
			if ((s[j] == '<' && s[j + 1] == '<') || (s[j] == '>' && s[j + 1] == '>')) //si << ou >>
				return (2);
			return (1);
		}
		else if (s[j] == '\'' || s[j] == '\"') // Si on rencontre une quote, on la traite complètement
		{
			quote_char = s[j];
			j++; // Passer la quote ouvrante
			while (s[j] && s[j] != quote_char) // Parcourir jusqu'à la quote fermante
				j++;
			j++; // Passer la quote fermante
		}
		else// Si caractère normal, on continue
			j++;
	}
	return (j);
}
/***********************************
*    verifie quelle est la quote   *
************************************/
//@Trouve le type de quote du premier caractère de str.
// @param str La chaîne de caractères à analyser
// @return 1 si = quote simple, 2 si = quote double, 0 si != quote
int	ft_get_quote_type(const char *str)
{
	if (*str == '\'')					// *str == str[0]
		return (1); // 1 pour quote simple
	if (*str == '\"')
		return (2); // 2 pour quote double
	return (0); // 0 si pas de quote
}
/***********************************
*     Attribue le type au token    *
************************************/
//t_token_type	ft_get_token_type(t_token *data)
t_token_type	ft_get_token_type(const char *data, int len)
{											//data == str
// D'abord les tokens de 2 caractères (pour éviter les faux positifs)
	if (data[len - 1] == '>')
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(data, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(data, "||", 2) == 0)
		return (TOK_AND);
	if (ft_strncmp(data, "&&", 2) == 0)
		return (TOK_OR);
// Puis les tokens de 1 caractère
	if (ft_strncmp(data, "|", 1) == 0)
		return (TOK_PIPE);
	if (ft_strncmp(data, ">", 1) == 0)
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, "<", 1) == 0)
		return (TOK_REDIR_IN);
	if (ft_strncmp(data, "$", 1) == 0)
		return (TOK_DOLLAR);
	if (ft_strncmp(data, "&", 1) == 0)
		return (TOK_AMPERSAND);
	if (ft_strncmp(data, "(", 1) == 0)
		return (TOK_PARA_OPEN);
	if (ft_strncmp(data, ")", 1) == 0)
		return (TOK_PARA_CLOSE);
	return (TOK_WORD);
}/**************************************
*				BONUS				  * 
* 	   Attribue le type au token      *
***************************************/
/*t_token_type	ft_bonus_get_token_type(const char *data)
{
	if (ft_strncmp(data, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(data, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(data, "&&", 2) == 0)
		return (TOK_AND);
	if (ft_strncmp(data, "||", 2) == 0)
		return (TOK_OR);
	if (ft_strncmp(data, "|", 1) == 0)
		return (TOK_PIPE);
	if (ft_strncmp(data, ">", 1) == 0)
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, "<", 1) == 0)
		return (TOK_REDIR_IN);
	if (ft_strncmp(data, "$", 1) == 0)
		return (TOK_DOLLAR);
	if (ft_strncmp(data, "&", 1) == 0)
		return (TOK_AMPERSAND);
	if (ft_strncmp(data, "(", 1) == 0)
		return (TOK_PARA_BEGIN);
	if (ft_strncmp(data, ")", 1) == 0)
		return (TOK_PARA_END);
	
	return (TOK_WORD);
}*/

/***********************************
*           Cree le token          *
*   = Crée des données du noeud    *
************************************/
//@Crée un nouveau token à partir de la chaîne str et de sa longueur len.
// @param str La chaîne de caractères à partir de laquelle créer le token
// @param len La longueur du token à créer
// @return Un pointeur vers le nouveau token ou NULL en cas d'erreur
t_token *ft_creat_token(const char *str, int len)
{												// Crée des données du noeud
    t_token *new_token;

	new_token = malloc(sizeof(t_token));
    if (new_token == NULL)
    {
		perror("Memory allocation error for ft_creat_token");
		return (NULL);
	}
	ft_memset(new_token, 0, sizeof(t_token));
    new_token->type = ft_get_token_type(str, len);
	//if (data) deja verifié dans strlen et strndup
//*** ce n'est pas new_token->quote_type = is_quote(str)*/
	new_token->quote_type = ft_get_quote_type(str);//verification sur le debut de str
	new_token->data = ft_strndup_token(str, len);
	if (new_token->data == NULL)
	{
		perror("Memory allocation error for token data");
		free(new_token);
		return (NULL);
	}
    return (new_token);
}
/********************************
*			BONUS			    * 
*     	 Cree le token          *
*********************************/
/*t_token *bonus_create_token(const char *str, int len)
{
    t_token *new_token;

	new_token = malloc(sizeof(t_token));
//VERIFIER LIBERATION PLUS TARD DE CE MALLOC
    if (new_token == NULL)
    {
		perror("Memory allocation error for create_liste");
		exit(EXIT_FAILURE);
	}
    new_token->type = ft_bonus_get_token_type(str);
	//if (data) deja verifié dans strlen et strndup
	new_token->quote_type = ft_get_quote_type(str);//verification sur le debut de str
	new_token->data = ft_strndup__token(str, len);
//DOIT ON VERIFIER SI DATA EST VIDE?
	if (new_token->data == NULL)
	{
		perror("Memory allocation error for token data");
		free(new_token);
		exit(EXIT_FAILURE);
	}
    return (new_token);
}*/
/****************************************************************************************
*	Creation de la liste chainée                                                        *
*	Appel ft_token_len (longeur du futur token)                                         *
*			is_in_charset                                                               *
*		  ft_creat_token (creation du token)								--> MALLOC  *
*		  	ft_strndup_token (token data = chaine copié dans le token)		--> MALLOC  *
*			ft_get_token_type (token type)                                              *
*			ft_get_quote_type (token quote_type)                                        *
*	 	  ft_add_node (cree un nveau noeud en fin de liste ou cree la liste)            *
*			ft_create_node (cree un nouveau noeud de liste)					--> MALLOC  *
*			ft_memset (initialise à zero le noeud de liste)                             *
*****************************************************************************************/
//@Divise la chaîne str en tokens en utilisant charset comme séparateur
// @param str La chaîne de caractères à diviser
// @param charset La chaîne de caractères définissant les séparateurs
// @return Une liste chaînée de tokens ou NULL en cas d'erreur
t_list	*tokenize_the_hell(char *str, char *charset)
{
	t_list	*token_list;// Liste pour stocker les tokens
	t_list *tmp_list;
	t_token *new_token;//structure pour chaque token
	size_t next_len;//necessité de garder valeur ft_next(str, charset, &meta)
					// car stndup appel ft_next donc ft_next change de valeur
	t_meta	meta;

	if (!str || !charset)//voir si on garde
		return (NULL);
	token_list = NULL; // Initialisation de la liste
	ft_init_meta(&meta);
	while (*str)
	{
		while (*str && (*str == ' ' || *str == '\t'))//si str debute par space
			str++;//avance pour creation du prochain token
		if (*str == '\0') // Si on arrive à la fin après avoir ignoré les espaces
			break ;
		next_len = ft_token_len(str, charset, &meta);//recupere la taille du token suivant
		if (next_len == 0)
			break ;
		new_token = ft_creat_token(str, next_len);// Crée un nouveau token
		if (!new_token) // Vérification d'erreur
		{
			free_token_list(token_list); // Libère la liste de tokens déjà créée
			break ;
			//return (NULL); // <-- retourne NULL si la création échoue
		}
		tmp_list = ft_add_node(token_list, new_token);// Crée un nouveau nœud de liste et l'ajoute à la liste ou cree la liste
		if (!tmp_list)//echec de creat_node via add_node
		{
			free_token_struct(new_token); // Libère la structure token
			free_token_list(token_list);
			return (NULL); // Si l'ajout échoue, retourne NULL
		}
		token_list = tmp_list;

		str += next_len;
	}
	if (token_list == NULL) // Si aucun token n'a été créé
	{
		printf("aucun tokens created\n");
		//return (NULL); // Retourne NULL si la liste est vide
	}
	return (token_list);
}
/*void free_token_struct(t_token *token)
{
	if (!token)
		return ;
	free(token->data); // Libère la chaîne de caractères du token
	free(token); // Libère la structure token
}
void	free_token_list(t_list *token_list)
{
	t_list *current;
	t_list *next;
	t_token *token;

	current = token_list;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->data;
		if (token)
		{
			if (token->data)
				free(token->data);  // Libère la chaîne de caractères du token
			free(token);  // Libère la structure token
		}
		free(current);  // Libère le nœud de la liste
		current = next;
	}
}*/
