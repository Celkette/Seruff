/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:07:59 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 14:47:39 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_enum.h"
# include "minishell_struct.h"
# include "minishell_fun.h"
# include "minishell_messages.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <curses.h>
# include <term.h>
# include <dirent.h>
# include <errno.h>
# include <features.h>
# include <stdbool.h>//
# include <sys/wait.h>
# include <dirent.h> // Pour le type DIR



#endif

//#ifndef COMMON_H
//# define COMMON_H