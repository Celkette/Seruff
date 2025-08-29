/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:27:08 by celine            #+#    #+#             */
/*   Updated: 2025/08/24 13:12:52 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/***************************************
*   verifie si char est un separateur  *
****************************************/
//@Vérifie si le caractère c appartient à la chaîne charset.
// @param c Le caractère à vérifier
// @param charset La chaîne de caractères dans laquelle vérifier
// @return 1 si c est dans charset, 0 sinon
int	is_in_charset(char c, char *charset)
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
*    verifie quelle est la quote   *
************************************/
//@Trouve le type de quote du premier caractère de str.
// @param str La chaîne de caractères à analyser
// @return 1 si = quote simple, 2 si = quote double, 0 si != quote
int	ft_get_quote_type(const char *str)
{
	if (*str == '\'')
		return (1);
	if (*str == '\"')
		return (2);
	return (0);
}

/***********************************
*     Attribue le type au token    *
************************************/
t_tkty	ft_get_token_type(const char *data, int len)
{
	if (data[len - 1] == '>')
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(data, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(data, ">", 1) == 0)
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, "<", 1) == 0)
		return (TOK_REDIR_IN);
	if (ft_strncmp(data, "|", 1) == 0)
		return (TOK_PIPE);
	return (TOK_WORD);
}

/***********************************
*           Cree le token          *
*   = Crée des données du noeud    *
************************************/
//@Crée un nouveau token à partir de la chaîne str et de sa longueur len.
// @param str La chaîne de caractères à partir de laquelle créer le token
// @param len La longueur du token à créer
// @return Un pointeur vers le nouveau token ou NULL en cas d'erreur
t_token	*ft_creat_token(const char *str, int len)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
	{
		perror("Memory allocation error for ft_creat_token");
		return (NULL);
	}
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->type = ft_get_token_type(str, len);
	new_token->quote_type = ft_get_quote_type(str);
	new_token->data = ft_strndup_token(str, len);
	if (new_token->data == NULL)
	{
		perror("Memory allocation error for token data");
		free(new_token);
		return (NULL);
	}
	return (new_token);
}
