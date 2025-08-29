/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_messages.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 07:37:22 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/24 17:08:08 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_MESSAGES_H
# define MINISHELL_MESSAGES_H

# define ERROR_PIPE1 "minishell: syntax error near unexpected token `|'\n"
# define ERROR_PIPE2 "minishell: syntax error near unexpected token `||'\n"
# define ERROR_NEWLINE "minishell: syntax error near unexpected \
token `newline'\n"
# define ERROR_NEWLINE2 "minishell: syntax error near unexpected \
token `newline'"
# define ERROR_QUOTE "minishell: syntax error miss quote\n"
# define ERROR_TOKEN1 "minishell: syntax error near unexpected token: `\n"
# define ERROR_TOKEN2 "minishell: syntax error near unexpected token: `"
# define CHARSET_1 "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define CHARSET_2 "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJK\
LMNOPQRSTUVWXYZ0123456789"
# define CHARSET_3 "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJK\
LMNOPQRSTUVWXYZ0123456789?$"
# define ERROR_ENV "shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n"
#endif