/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:54:11 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 15:50:30 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*is_an_redir(char c)
Vérifie si le caractère passé est une redirection (< ou >). 
//Retourne 1 si oui, sinon 0.

*check_in_1(t_list lst)
Vérifie la validité d’une redirection d’entrée (<).

Gère les erreurs de syntaxe (ex : < suivi de > ou double <).
Ouvre le fichier en lecture si la syntaxe est correcte.
Affiche des messages d’erreur en cas de problème (fichier inexistant, 
//mauvais token, etc.).
*check_out_1(t_list lst)
Vérifie la validité d’une redirection de sortie (>).

Gère les erreurs de syntaxe (ex : > suivi de |).
Ouvre/crée le fichier en écriture si la syntaxe est correcte.
Affiche des messages d’erreur en cas de problème.
*check_redir_case(t_list lst)
Parcourt la liste de tokens et appelle check_in_1 ou check_out_1 selon 
//le type de redirection rencontré.

Retourne une erreur si une redirection est mal formée.
*check_redir(t_list lst)
Fonction principale qui lance la vérification des redirections sur la 
//liste de tokens en appelant check_redir_case.
  */
// Verifie si le "token" est une redirection
int	is_an_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

static int	check_in_1_next(t_list *lst, int fd)
{
	if (!lst)
		return (-1);
	if (((t_token *)lst->data)->data[0] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token: `", 2);
		ft_putstr_fd(((t_token *)lst->data)->data, 2);
		ft_putendl_fd("'", 2);
		return (1);
	}
	fd = open(((t_token *)lst->data)->data, O_RDONLY, 0777);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(((t_token *)lst->data)->data, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	close(fd);
	return (0);
}

static void	print_check_in_1(t_list *lst)
{
	ft_putstr_fd(ERROR_TOKEN2, 1);
	ft_putstr_fd(((t_token *)lst->data)->data, 1);
	ft_putstr_fd("'\n", 1);
}

int	check_in_1(t_list *lst)
{
	int	fd;

	if (!lst)
		return (-1);
	fd = -1;
	if (ft_strlen(((t_token *)lst->data)->data) == 1 && \
		((t_token *)lst->data)->data[0] == '<' && lst->next && \
		((t_token *)lst->next->data)->data[0] == '>')
	{
		ft_putstr_fd(ERROR_TOKEN2, 2);
		ft_putstr_fd(((t_token * )lst->next->data)->data, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	if (ft_strlen(((t_token *)lst->data)->data) == 2 && \
		((t_token *)lst->data)->data[0] == '<' && lst->next && \
		((t_token *)lst->data)->data[1] == '<')
	{
		return (-1);
	}
	if (lst->next && !is_an_redir(((t_token *)lst->next->data)->data[0]))
		return (check_in_1_next(lst->next, fd));
	else
	{
		ft_putstr_fd(ERROR_TOKEN2, 2);
		ft_putstr_fd(((t_token * )lst->data)->data, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	print_check_in_1(lst);
	return (1);
}


