/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:37:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/21 12:51:14 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_an_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

static int	check_out_1_2(t_list *lst, int fd)
{
	if (!lst)
		return (-1);
	if (((t_token *)lst->data)->data[0] == '|')
	{
		ft_putstr_fd(ERROR_TOKEN2, 1);
		ft_putstr_fd(((t_token *)lst->data)->data, 1);
		ft_putendl_fd("'", 1);
		return (1);
	}
	if (!is_an_redir(((t_token *)lst->data)->data[0]))
	{
		fd = open(((t_token *)lst->data)->data, O_CREAT | O_WRONLY, 0777);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 1);
			ft_putstr_fd(((t_token *)lst->data)->data, 1);
			ft_putstr_fd(": ", 1);
			ft_putendl_fd(strerror(errno), 1);
			return (-1);
		}
		close(fd);
		return (0);
	}
	return (-1);
}

// Verifie les redirections exterieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il affiche une erreur
// si la case du tableau est OK il l'ouvre en creant le fichier
// si il open echou il return une erreur sinon il ferme fd et quit
int	check_out_1(t_list *lst)
{
	int	fd;

	if (!lst)
		return (-1);
	fd = -1;
	if (lst->next)
	{
		lst = lst->next;
		if (check_out_1_2(lst, fd) == 0)
			return (0);
	}
	else
	{
		ft_putendl_fd(ERROR_NEWLINE2, 1);
		return (-1);
	}
	ft_putstr_fd(ERROR_TOKEN2, 1);
	ft_putstr_fd(((t_token *)lst->data)->data, 2);
	ft_putendl_fd("'", 1);
	return (1);
}

// Verifie les redirections interieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il passe au prochain
// si la case du tableau est OK il se stop
// si il ne trouve pas de "token" valide (que des redirection)
// il affiche une erreur
int	check_redir(t_list *lst)
{
	int		code;

	(void)code;
	if (!lst)
		return (-2);
	while (lst)
	{
		if (((t_token *)lst->data)->data && \
			((t_token *)lst->data)->data[0] == '<')
			if (check_in_1(lst) != 0)
				return (-1);
		if (((t_token *)lst->data)->data && \
			((t_token *)lst->data)->data[0] == '>')
			if (check_out_1(lst) != 0)
				return (-1);
		lst = lst->next;
	}
	return (0);
}
