# LIBFT
SOURCES		= main.c \
			  src/build_in/cd/cd.c \
			  src/build_in/cd/cd2.c \
			  src/build_in/cd/cd3.c \
			  src/build_in/export/export.c \
			  src/build_in/export/export2.c \
			  src/build_in/export/export3.c \
			  src/build_in/export/export4.c \
			  src/build_in/echo.c \
			  src/build_in/env.c \
			  src/build_in/pwd.c \
			  src/build_in/unset.c \
			  src/build_in/utils.c \
			  src/exit/exit.c \
			  src/init/init_env.c \
			  src/init/init_env2.c \
			  src/init/init_struct_meta.c \
			  src/lexing/lexing_check_syntaxe.c \
			  src/lexing/lexing_flag.c \
			  src/lexing/lexing_util.c \
			  src/lexing/redirection.c \
			  src/lexing/redirection2.c \
			  src/parsing/expand/expand.c \
			  src/parsing/expand/expand2.c \
			  src/parsing/expand/expand_util.c \
			  src/parsing/expand/expand_util2.c \
			  src/parsing/local_var/get_env_var.c \
			  src/parsing/local_var/get_env_var2.c \
			  src/parsing/local_var/local_var_decl.c \
			  src/parsing/local_var/local_var_decl2.c \
			  src/parsing/local_var/local_var_len.c \
			  src/parsing/local_var/local_var_list.c \
			  src/parsing/local_var/local_var_name.c \
			  src/parsing/local_var/local_var_value.c \
		  	  src/parsing/local_var/update_local_var.c \
			  src/parsing/delete_quote.c \
			  src/sig/init.c \
			  src/sig/signals.c \
			  src/tree/exec/one_cmd/exec_one_cmd.c \
			  src/tree/exec/one_cmd/redir_cmd.c \
			  src/tree/exec/one_cmd/utils.c \
			  src/tree/exec/pipeline/exec_pipeline.c \
			  src/tree/exec/pipeline/utils.c \
			  src/tree/exec/exec.c \
			  src/tree/node/node.c \
			  src/tree/node/utils.c \
			  src/tree/node/utils2.c \
			  src/tree/prepare/prepare.c \
			  src/tree/prepare/utils.c \
			  src/tree/redir/redir.c \
			  src/tree/redir/redir2.c \
			  src/tree/redir/utils.c \
			  src/tree/free.c \
			  src/tree/heredoc.c \
			  src/utils/gnl/get_next_line.c\
			  src/utils/gnl/get_next_line_utils.c \
			  src/utils/linked_list/free_element.c \
			  src/utils/linked_list/free_element2.c \
			  src/utils/linked_list/linked_list_util.c \
			  src/utils/linked_list/linked_list_util2.c \
			  src/utils/ft_atoi.c \
			  src/utils/ft_isalnum.c \
			  src/utils/ft_isalpha.c \
			  src/utils/ft_itoa.c \
			  src/utils/ft_lstadd_back.c \
			  src/utils/ft_lstadd_front.c \
			  src/utils/ft_lstsize.c \
			  src/utils/ft_memcpy.c \
			  src/utils/ft_memset.c \
			  src/utils/ft_putendl_fd.c  \
			  src/utils/ft_split.c \
			  src/utils/ft_strchr.c \
			  src/utils/ft_strdup.c \
			  src/utils/ft_strjoin.c \
			  src/utils/ft_strlen.c \
			  src/utils/ft_strncmp.c \
			  src/utils/ft_strndup.c \
			  src/utils/ft_strrchr.c \
			  src/utils/ft_strstr.c  \
			  src/utils/tokenize_util.c \
			  src/utils/tokenize1.c \
			  src/utils/tokenize2.c \
			  src/read_stdin.c \
			  src/read_stdin2.c \
			  wait_exec.c \
			  tokenize.c \
			  src/tree/redir/redir_heredoc.c \
			  src/tree/exec/one_cmd/check_cmd_exec.c \
			  src/exit/exit_built_in.c \
			  src/lexing/check_redir.c \

OBJECTS     = ${SOURCES:.c=.o}
NAME        = minishell
CC          = cc
FLAGS       = -Wall -Werror -Wextra -g
CFLAG		= -lreadline -lcurses
#-lreadline = lie la bibliothèque readline
#Fournit des fonctions pour lire une ligne de texte avec édition interactive (flèches, historique, complétion…).
#-lcurses = Lie la bibliothèque curses
#ermet de gérer l’affichage avancé dans le terminal (fenêtres, couleurs, gestion du curseur…).
#Remarque : readline dépend parfois de ncurses pour fonctionner correctement, d’où la présence de -lcurses.GREEN   = \033[92m
BLUE    = \033[94m
ORANGE  = \033[93m
RED     = \033[91m
BOLD            = \033[1m
WHITE   = \033[0m

all: ${NAME}

${NAME}: ${OBJECTS}
	${CC} -o ${NAME} ${OBJECTS} ${CFLAG}

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJECTS}

fclean: clean
	rm -rf ${NAME}

re: fclean all

#penser a supprimer
valgrind: $(NAME)
	valgrind --leak-check=full --track-origins=yes ./$(NAME)

.PHONY: all clean fclean re

#src/lexing/lexing_check_syntaxe.c src/lexing/redirection.c
