/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:11:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 15:48:45 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_cmd_arg(char *cmd)
{
	char	**cmd_args;

	cmd_args = NULL;
	cmd_args = pipex_ft_split(cmd, ' ');
	if (cmd_args == NULL)
		pipex_error_i("cmd_args = ft_split failed (in get_cmd_arg)", -1);
	return (cmd_args);
}
