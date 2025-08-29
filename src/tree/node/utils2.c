/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:55:30 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 23:13:24 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// permet d'init un node de tree avec une valeur
// type == 0 pour une commande et 1 pour un operateur
t_tree	*init_node(t_token *token, int type)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_tree));
	if (!token)
		node->content = NULL;
	else
		node->content = ft_strndup((char *)token->data, \
		ft_strlen((char *)token->data));
	node->type = type;
	return (node);
}

static int	print_error_tree(char *s, t_tree *node)
{
	ft_putstr_fd("minishell: ", 2);
	if (s)
		ft_putstr_fd(s, 2);
	else
		ft_putstr_fd(" ", 2);
	ft_putendl_fd(": command not found", 2);
	if (node)
		node->path = NULL;
	return (127);
}

int	node_slash(char *content)
{
	int	len;

	len = ft_strlen(content);
	if ((len > 0 && content[len] != '/') || len == 0)
		return (1);
	return (0);
}

// Si dans la commande on rencontre un tok_word sans redirection
// Si c une commande on cree ajout la commande a content 
// (contient la commande a exec) de tree
// Sinon on lajoute dans la list des arguments
int	manage_token_word(t_env *minishell, t_tree *node, \
	t_token *token, int *has_cmd)
{
	char	*new_content;
	int		code;

	new_content = NULL;
	code = 0;
	if (*has_cmd == 0)
	{
		node->built_in = is_built_in(minishell, node, token->data);
		if (!node->built_in)
			new_content = cmd_is_in_path(minishell, token->data);
		if (!new_content && !node->built_in && node_slash(node->content) == 1)
			code = (print_error_tree(token->data, node));
		else if (!node->built_in)
			node->path = new_content;
		if (node->content)
			free(node->content);
		node->content = ft_strndup((char *)token->data, \
		ft_strlen((char *)token->data));
		*has_cmd = 1;
	}
	else
		ft_lstadd_back(&node->arg, \
			ft_create_node(ft_strndup((char *)token->data, \
			ft_strlen((char *)token->data))));
	return (code);
}
