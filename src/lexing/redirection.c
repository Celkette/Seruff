/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:54:11 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 01:37:08 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// Verifie si le "token" est une redirection
int		is_an_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int		check_in_1(t_list *lst)
{
	int	fd;
	
	if (!lst)
	return (-1);
	if (ft_strlen(((t_token *)lst->data)->data) == 1 && ((t_token *)lst->data)->data[0] == '<' && lst->next && ((t_token *)lst->next->data)->data[0] == '>')
	return printf("minishell: syntax error near unexpected token `newline'\n");
	if (lst->next)
	{
		lst = lst->next;
		if (!is_an_redir(((t_token *)lst->data)->data[0]))
		{
			if (((t_token *)lst->data)->data[0] == '|')
			{
				printf("minishell: syntax error near unexpected token: `%s'\n", ((t_token *)lst->data)->data);
				return (1);
			}
			fd = open(((t_token *)lst->data)->data, O_RDONLY, 0777);
			if (fd < 0)
				return printf("minishell: %s: %s\n", ((t_token *)lst->data)->data, strerror(errno));
			close(fd);
			return (0);
		}
	}
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	printf("minishell: syntax error near unexpected token `%s'\n", ((t_token *)lst->data)->data);
	return (1);
}

// Verifie les redirections exterieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il affiche une erreur
// si la case du tableau est OK il l'ouvre en creant le fichier
// si il open echou il return une erreur sinon il ferme fd et quit
int		check_out_1(t_list *lst)
{
	int	fd;

	if (!lst)
		return (-1);
	if (lst->next)
	{
		lst = lst->next;
		if (((t_token *)lst->data)->data[0] == '|')
		{
			printf("minishell: syntax error near unexpected token `%s'\n", ((t_token *)lst->data)->data);
			return (1);
		}
		if (!is_an_redir(((t_token *)lst->data)->data[0]))
		{
			fd = open(((t_token *)lst->data)->data, O_CREAT | O_WRONLY, 0777);
			if (fd < 0)
				return printf("minishell: %s: %s\n", ((t_token *)lst->data)->data, strerror(errno));
			close(fd);
			return (0);
		}
	}
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	printf("minishell: syntax error near unexpected token `%s'\n", ((t_token *)lst->data)->data);
	return (1);
}

// Verifie les redirections interieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il passe au prochain
// si la case du tableau est OK il se stop
// si il ne trouve pas de "token" valide (que des redirection)
// il affiche une erreur

// Verifie si le token est une redirection '<' ou '>' et redirige
int		check_redir_case(t_list *lst)
{
	if (!lst)
		return (-2);
	while (lst)
	{
		if (((t_token *)lst->data)->data[0] == '<')
			if (check_in_1(lst) != 0)
				return (-1);
		if (((t_token *)lst->data)->data[0] == '>')
			if (check_out_1(lst) != 0)
				return (-1);
		lst = lst->next;
	}
	return (0);
}

int		check_redir(t_list *lst)
{
	int		code;
	
	if (!lst)
		return (-1);
	code = check_redir_case(lst);
	return (code);
}
