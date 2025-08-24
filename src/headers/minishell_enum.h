/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_enum.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:09:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/23 09:20:39 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENUM_H
# define MINISHELL_ENUM_H

// DEFINE POUR CHARGER LE PROMPT DE READLINE 0 = classic 1 = special
# define SPECIAL_PROMPT 0

//enum e_tok_redirect
//{
//	PIPE = '|',
//	RED_OUT = '>',
//	RED_IN = '<'
//};

//enum e_tok_sub_shell
//{
//	BEGIN_PARA = '(',
//	END_PARA = ')'
//};

//enum e_tok_space
//{
//	SPACE_CLASSIC = (char)32,
//	SPACE_TAB = '\t',
//	NEW_LINE = '\n'
//};

//enum e_tok_string
//{
//	QUOTE = (char)39,
//	D_QUOTE = '"'
//};

//typedef enum e_token_type
typedef enum e_t_tkty
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_OUT,
	TOK_REDIR_IN,
	TOK_REDIR_APPEND,
	TOK_REDIR_HEREDOC,
	TOK_DOLLAR,
	TOK_AMPERSAND,
	TOK_PARA_OPEN,
	TOK_PARA_CLOSE,
	TOK_AND,
	TOK_OR,
}			t_tkty;

#endif