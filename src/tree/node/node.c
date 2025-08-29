/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:55:55 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/13 16:23:20 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Si il y a une redirection dans la token commande
// On recupere la redirection qui correspond avec les fonctions de redir.c
int	manage_token_redir(t_env *minishell, t_list *head, \
	t_tree *node, t_token *token)
{
	void	*tmp;

	tmp = NULL;
	if (!head || !node || !token)
		return (-2);
	if (token->type == TOK_REDIR_IN || token->type == TOK_REDIR_HEREDOC)
	{
		tmp = get_redir_in(minishell, head, node);
		if (!tmp)
			return (-1);
		if (node->redir_in)
			free_redir_in(node->redir_in);
		else if (node->redir_in && node->redir_in->fd == -1)
			return ((free_redir_in(node->redir_in), free(tmp)), -3);
		node->redir_in = tmp;
	}
	else if (token->type == TOK_REDIR_OUT || token->type == TOK_REDIR_APPEND)
	{
		tmp = get_redir_out(head);
		if (!tmp)
			return (-2);
		ft_lstadd_back(&node->redir_out, ft_create_node(tmp));
	}
	return (1);
}

static int	check_for_redir(t_env *minishell, t_list *head, \
	t_tree *node, t_token *token)
{
	if (token->type == TOK_REDIR_IN || token->type == TOK_REDIR_OUT || \
		token->type == TOK_REDIR_HEREDOC || token->type == TOK_REDIR_APPEND)
	{
		if (manage_token_redir(minishell, head, node, token) < 0)
		{
			free_tree(node);
			return (-1);
		}
	}
	return (1);
}

static int	is_a_valid_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOK_REDIR_IN || token->type == TOK_REDIR_OUT || \
		token->type == TOK_REDIR_HEREDOC || \
		token->type == TOK_REDIR_APPEND || \
		token->type == TOK_WORD)
		return (1);
	return (0);
}

static int	while_for_token(t_env *minishell, t_list *head, \
	int *has_cmd, t_tree *node)
{
	t_token	*token;
	int		code;

	if (!minishell || !head)
		return (-1);
	while (head)
	{
		token = see_token(head);
		if (!token || !is_a_valid_token(token))
			break ;
		code = check_for_redir(minishell, head, node, token);
		if (code == -1)
			return (-1);
		else if (token->type == TOK_WORD)
		{
			token = consume_token(head);
			if (!token)
				return ((free_tree(node), -1));
			code = manage_token_word(minishell, node, token, has_cmd);
			if (code < 0)
				return (free_tree(node), -1);
		}
		head = head->next;
	}
	return (1);
}

// Permet de creer un node darbre a partir dune liste de token word et redir
// Il init un node sans commande dedans, puis parcour la liste de token
// si le token est une redirection on passe 
// par la fonction qui gere les tokens redirections
// si le token est un mot on passe par la fonction qui gere les tokens words
// si le token ne correspond pas, on retourne le node creer
t_tree	*create_word_node_tree(t_env *minishell, t_list *head, int *valid)
{
	t_token	*token;
	t_tree	*node;
	int		has_cmd;

	if (!head || *valid < 0)
		return (NULL);
	token = see_token(head);
	if (!token)
		return (NULL);
	if (token->type != TOK_REDIR_IN && token->type != TOK_REDIR_OUT && \
		token->type != TOK_WORD && token->type != TOK_REDIR_HEREDOC && \
		token->type == TOK_REDIR_APPEND)
		return (NULL);
	node = init_node(NULL, 0);
	if (!node)
		return (NULL);
	has_cmd = 0;
	*valid = while_for_token(minishell, head, &has_cmd, node);
	if (!valid)
	{
		free(node);
		return (NULL);
	}
	return (node);
}
