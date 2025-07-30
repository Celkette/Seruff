/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:55:55 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 05:57:25 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static	char *cmd_is_in_path(t_env *minishell, char *cmd)
{
	char	*tmp_path;
	int		i;

	if (!minishell || !cmd)
		return (NULL);
	i = 0;
	while (minishell->path_fun_split[i])
	{
		if (ft_strchr(cmd, '/'))
		{
			if (access(cmd, X_OK) < 0)
				return (NULL);
			return (cmd);
		}
		else
		{
			tmp_path = malloc(sizeof(char) * (ft_strlen(minishell->path_fun_split[i]) + ft_strlen(cmd) + 2));
			if (!tmp_path)
				return (NULL);
			ft_memcpy(tmp_path, minishell->path_fun_split[i], ft_strlen(minishell->path_fun_split[i]));
			*(tmp_path + ft_strlen(minishell->path_fun_split[i])) = '/';
			ft_memcpy(&tmp_path[ft_strlen(minishell->path_fun_split[i]) + 1], cmd, ft_strlen(cmd));
			*(tmp_path + ft_strlen(minishell->path_fun_split[i]) + ft_strlen(cmd) + 1) = 0;
			if (access(tmp_path, X_OK) == 0)
				break;
			free(tmp_path);
			tmp_path = NULL;
		}
		i++;
	}
	if (tmp_path == NULL)
		return (NULL);
	return (tmp_path);
}

// permet d'init un node de tree avec une valeur
// type == 0 pour une commande et 1 pour un operateur
t_tree	*init_node(t_token *token, int type)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_tree));
	if (!token)
		node->content = NULL;
	else
		node->content = ft_strndup((char *)token->data, ft_strlen((char *)token->data));
	node->type = type;
	return (node);
}

// fonction qui retourne un pointeur de fonction dune commande built_in
static int (*is_built_in(t_env *minishell, char *str))(void *p)
{
	if (ft_strncmp(str, "echo", ft_strlen("echo") + 1) == 0)
		return (&echoo);
	else if (ft_strncmp(str, "cd", ft_strlen("cd") + 1) == 0)
		return (&cd);
	else if (ft_strncmp(str, "pwd", ft_strlen("pwd") + 1) == 0)
		return (&pwd);
	else if (ft_strncmp(str, "export", ft_strlen("export") + 1) == 0)
		return (NULL);
	else if (ft_strncmp(str, "unset", ft_strlen("unset") + 1) == 0)
		return (NULL);
	else if (ft_strncmp(str, "env", ft_strlen("env") + 1) == 0)
		return (&env);
	else if (ft_strncmp(str, "exit", ft_strlen("exit") + 1) == 0)
	{
		exit_minishell(minishell);
		exit(0);
	}
	return (NULL);
}

// Si dans la commande on rencontre un tok_word sans redirection
// Si c une commande on cree ajout la commande a content (contient la commande a exec) de tree
// Sinon on lajoute dans la list des arguments
int	manage_token_word(t_env *minishell, t_tree *node, t_token *token, int *has_cmd)
{
	char	*new_content;

	if (*has_cmd == 0)
	{
		node->built_in = is_built_in(minishell, token->data);
		if (!node->built_in)
			new_content = cmd_is_in_path(minishell, token->data);
		if (!new_content && !node->built_in)
		{
			printf("minishell: command not found: %s\n", token->data);
			node->path = NULL;
			return (127);
		}
		else if (!node->built_in)
		{
			node->path = new_content;
		}
		node->content = ft_strndup((char *)token->data, ft_strlen((char *)token->data));
		*has_cmd = 1;
	}
	else
		ft_lstadd_back(&node->arg, ft_create_node(ft_strndup((char *)token->data, ft_strlen((char *)token->data))));
	return (0);
}

// Si il y a une redirection dans la token commande
// On recupere la redirection qui correspond avec les fonctions de redir.c
int manage_token_redir(t_list *head, t_tree *node, t_token *token)
{
	void	*tmp;

	tmp = NULL;
	if (!head || !node || !token)
		return (-2);
	if (token->type == TOK_REDIR_IN || token->type == TOK_REDIR_APPEND)
	{
		tmp = get_redir_in(head);
		if (!tmp)
		{
			return (-1);
		}
		if (node->redir_in)
			free(node->redir_in);
		node->redir_in = tmp;
	}
	else if (token->type == TOK_REDIR_OUT || token->type == TOK_REDIR_HEREDOC)
	{
		tmp = get_redir_out(head);
		if (!tmp)
		{
			return (-2);
		}
		ft_lstadd_back(&node->redir_out, ft_create_node(tmp));
	}
	return (0);
}

// Permet de creer un node darbre a partir dune liste de token word et redir
// Il init un node sans commande dedans, puis parcour la liste de token
// si le token est une redirection on passe par la fonction qui gere les tokens redirections
// si le token est un mot on passe par la fonction qui gere les tokens words
// si le token ne correspond pas, on retourne le node creer
t_tree	*create_word_node_tree(t_env *minishell, t_list *head, int *valid)
{
	t_token	*token;
	t_tree	*node;
	int		has_cmd;

	if (!head)
		return (NULL);
	token = see_token(head);
	if (!token)
		return (NULL);
	if (token->type != TOK_REDIR_IN && token->type != TOK_REDIR_OUT && token->type != TOK_WORD && token->type != TOK_REDIR_HEREDOC && token->type == TOK_REDIR_APPEND)
		return (NULL);
	node = init_node(NULL, 0);
	if (!node)
		return (NULL);
	has_cmd = 0;
	while (head)
	{
		token = see_token(head);
		if (!token)
			break;
		if (token->type == TOK_REDIR_IN || token->type == TOK_REDIR_OUT || token->type == TOK_REDIR_HEREDOC || token->type == TOK_REDIR_APPEND)
		{
			if (manage_token_redir(head, node, token) < 0)
			{
				// FUN POUR ABORT
				*valid = -1;
				return (NULL);
			}
		}
		else if (token->type == TOK_WORD)
		{
			consume_token(head);
			if (manage_token_word(minishell, node, token, &has_cmd) < 0)
			{
				return (NULL);
			}
		}
		else
			break;
		head = head->next;
	}
	return (node);
}
