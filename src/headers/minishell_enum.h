/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_enum.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:09:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/23 23:17:08 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENUM_H
# define MINISHELL_ENUM_H

// DEFINE POUR CHARGER LE PROMPT DE READLINE 0 = classic 1 = special
# define SPECIAL_PROMPT 0

// Enum pour tokenization, REDIRECTION
enum e_tok_redirect
{
	PIPE = '|',
	RED_OUT = '>',
	RED_IN = '<'
};

// Enum pour tokenization, SUB_SHELL
enum e_tok_sub_shell
{
	BEGIN_PARA = '(',
	END_PARA = ')'
};

// Enum pour tokenization, SPACE
enum e_tok_space
{
	SPACE_CLASSIC = (char)32,
	SPACE_TAB = '\t',
	NEW_LINE = '\n'
};

// Enum pour tokenization, STRING
enum e_tok_string
{
	QUOTE = (char)39,
	D_QUOTE = '"'
};

typedef enum e_token_type
{
	TOK_WORD,//				// Mot simple	0
	TOK_PIPE,//				|				1
	TOK_REDIR_OUT,//		<				2
	TOK_REDIR_IN,//	 		>				3
	TOK_REDIR_APPEND,//  	>>				4
	TOK_REDIR_HEREDOC,//  	<<				5
	TOK_DOLLAR,//			$				6
	TOK_AMPERSAND,//		&				7
//	BONUS	
	TOK_PARA_OPEN,//		(				8
	TOK_PARA_CLOSE,//	    )		        9
	TOK_AND,//				&&				10
	TOK_OR,//				||				11
		
}			t_token_type;

#endif
