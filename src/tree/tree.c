/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 07:08:50 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 10:01:45 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
 * Prat parsing

	https://www.youtube.com/watch?v=0c8b7YfsBKs
*/

// Retourne le prochain token inutilise dans l'arbre et change status a utilise
t_token	*consume_token(t_list *lst)
{
	t_token	*tmp;

	if (!lst)
		return 0;
	while (lst)
	{
		tmp = (t_token *)lst->data;
		if (!tmp->consume)
		{
			tmp->consume++;
			return (tmp);
		}
		lst = lst->next;
	}
	return 0;
}

// Retourne le prochain token inutilise dans l'arbre sans change status
t_token	*see_token(t_list *lst)
{
	t_token	*tmp;

	if (!lst)
		return 0;
	while (lst)
	{
		tmp = (t_token *)lst->data;
		if (!tmp->consume)
			return (tmp);
		lst = lst->next;
	}
	return 0;
}

/* Creer un arbre a partir dun liste de token
	https://www.youtube.com/watch?v=0c8b7YfsBKs
*/
t_tree *prepare(t_env *minishell, t_list *lst, int min_power, int *valid)
{
	t_token	*nud, *mid;
	t_tree	*nud_t, *mid_t, *tmp;
	int		power;

	(void)valid;
	if (!lst)
		return NULL;
	nud = see_token(lst);
	if (!nud)
		return NULL;
	if (nud->type == TOK_PARA_OPEN)
	{
		consume_token(lst);
		return prepare(minishell, lst, 0, valid);
	}
	else if (nud->type == TOK_REDIR_OUT || nud->type == TOK_REDIR_IN || nud->type == TOK_WORD || nud->type == TOK_REDIR_HEREDOC)
	{
		nud_t = create_word_node_tree(minishell, lst, valid);
		if (!nud_t)
		{
			// FUN ABORT
			while (nud)
			{
				if (nud->type != TOK_REDIR_OUT && nud->type != TOK_REDIR_IN && nud->type != TOK_WORD && nud->type != TOK_REDIR_HEREDOC)
					break;
				nud = consume_token(lst);
			}
			return (prepare(minishell, lst, 0, valid));
		}
	}
	else
	{
		nud = consume_token(lst);
		nud_t = init_node(nud, 1);
	}
	while (1)
	{
		mid = see_token(lst);
		if (!mid)
		{
			consume_token(lst);
			break;
		}
		if (mid->type == TOK_PARA_CLOSE)
		{
			consume_token(lst);
			break;
		}
		power = mid->type;
		if (power < min_power)
			break;
		if (mid->type == TOK_WORD || mid->type == TOK_REDIR_IN || mid->type == TOK_WORD || mid->type == TOK_REDIR_HEREDOC)
		{
			consume_token(lst);
			nud_t = create_word_node_tree(minishell, lst, valid);
		}
		else
			mid = consume_token(lst);
		mid_t = init_node(mid, 1);
		if (!mid_t || !nud_t)
			break;
		mid_t->left = nud_t;
		nud_t->top = mid_t;
		tmp = prepare(minishell, lst, power + 1, valid);
		if (!tmp)
			tmp = NULL;
		else
		tmp->top = mid_t;
		mid_t->right = tmp;
		nud_t = mid_t;
		if (mid->type == TOK_PARA_CLOSE)
		{
			consume_token(lst);
			nud_t = prepare(minishell, lst, 0, valid);
		}
	}
	return nud_t;
}

void print_tree(t_tree *tree, int level, char r)
{
	int	i;
	
	i = 0;
	if (!tree)
		return;
	while (i < level)
		printf(i++ == level - 1 ? "|-" : "  ");
	printf("%c: '%s'", r, tree->content);
	if (tree->arg)
	{
		t_list	*arg;
		arg = (t_list *)tree->arg;
		printf(" arg =>");
		while (arg)
		{
			printf(" %s ", (char *)arg->data);
			arg = arg->next;
		}
	}
	if (tree->redir_out)
	{
		t_list *tmp;

		tmp = tree->redir_out;
		printf(" redir out ");
		while (tmp)
		{
			printf(" %s fd=%i",(char *)((t_redir_out *)tmp->data)->path, (int)((t_redir_out *)tmp->data)->fd);
			tmp = tmp->next;
		}
		printf("\n");
	}
	if (tree->redir_in)
	{
		printf("redir_in <= %s\n", tree->redir_in->path);
	}
	printf("\n");
	print_tree(tree->left, level + 1, 'L');
	print_tree(tree->right, level + 1, 'R');
}

void	free_tree(t_tree *tree)
{
	t_list	*to_free;

	if (!tree)
		return ;
	if (tree->left)
		free_tree(tree->left);
	if (tree->right)
		free_tree(tree->right);
	while (tree->redir_out)
	{
		to_free = tree->redir_out;
		tree->redir_out = tree->redir_out->next;
		free(((t_redir_out *)to_free->data)->path);
		free(to_free->data);
		free(to_free);
	}
	while (tree->arg)
	{
		to_free = tree->arg;
		tree->arg= tree->arg->next;
		free(to_free->data);
		free(to_free);
	}
	if (tree->redir_in)
	{
		free(tree->redir_in->path);
		free(tree->redir_in);
	}
	if (tree->cmd_list)
		free_split(tree->cmd_list);
	if (tree->content)
		free(tree->content);
	if (tree->path)
		free(tree->path);
	free(tree);
}
