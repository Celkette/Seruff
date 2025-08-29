/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:42:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/27 14:25:02 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*****************************************************************************
*	Creation de la liste chainée                                             *
*	Appel ft_token_len (longeur du futur token)                              *
*			is_in_charset                                                    *
*		  ft_creat_token (creation du token)					             *
*		  	ft_strndup_token (token data = chaine copié dans le token)       *
*			ft_get_token_type (token type)                                   *
*			ft_get_quote_type (token quote_type)                             *
*	 	  ft_add_node (cree un nveau noeud en fin de liste ou cree la liste) *
*			ft_create_node (cree un nouveau noeud de liste)					 *
*			ft_memset (initialise à zero le noeud de liste)                  *
*****************************************************************************/
//@Divise la chaîne str en tokens en utilisant charset comme séparateur
// @param str La chaîne de caractères à diviser
// @param charset La chaîne de caractères définissant les séparateurs
// @return Une liste chaînée de tokens ou NULL en cas d'erreur
static int	add_tok_to_lst(char **s, char *charset, t_list **lst, t_meta *meta)
{
	size_t	next_len;
	t_token	*new_token;
	t_list	*tmp_list;

	next_len = ft_token_len(*s, charset, meta);
	if (next_len == 0)
		return (0);
	new_token = ft_creat_token(*s, next_len);
	if (!new_token)
	{
		free_token_list(*lst);
		return (0);
	}
	tmp_list = ft_add_node(*lst, new_token);
	if (!tmp_list)
	{
		free_token_struct(new_token);
		free_token_list(*lst);
		*lst = NULL;
		return (0);
	}
	*lst = tmp_list;
	*s += next_len;
	return (1);
}

t_list	*tokenize_the_hell(char *str, char *charset)
{
	t_list	*token_list;
	t_meta	meta;

	token_list = NULL;
	if (!str || !charset)
		return (NULL);
	ft_init_meta(&meta);
	while (*str)
	{
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		if (*str == '\0')
			break ;
		if (!add_tok_to_lst(&str, charset, &token_list, &meta))
			break ;
	}
	return (token_list);
}
