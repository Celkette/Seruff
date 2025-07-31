# LIBFT
SOURCES		= main.c src/read_stdin.c \
			  src/utils/linked_list/util_liste_chainee.c \
			  src/utils/ft_lstadd_back.c \
			  src/tree/node.c \
			  src/lexing/redirection.c \
			  src/lexing/lexing_check_syntaxe.c \
			  src/lexing/lexing_flag.c src/lexing/lexing_util.c \
			  src/sig/init.c src/sig/signals.c \
			  src/utils/ft_memset.c src/utils/ft_strlen.c \
			  src/utils/ft_strncmp.c src/utils/ft_split.c \
			  src/utils/gnl/get_next_line.c src/utils/gnl/get_next_line_utils.c \
			  src/utils/ft_strndup.c src/utils/ft_atoi.c src/utils/ft_strstr.c  \
			  src/utils/ft_strdup.c src/utils/ft_memcpy.c src/utils/ft_putendl_fd.c  \
			  src/utils/tokenize_the_hell.c src/utils/ft_strchr.c \
			  src/init/init_env.c \
			  src/init/init_struct_meta.c \
			  src/exit/exit.c \
			  src/build_in/cd/cd.c src/build_in/cd/cd2.c \
			  src/build_in/cd/cd_utils.c \
			  src/build_in/echo.c \
			  src/build_in/export.c \
			  src/build_in/unset.c \
			  src/build_in/pwd.c src/build_in/env.c \
			  src/parsing/expand/expand.c \
			  src/parsing/expand/expand_util.c \
			  src/parsing/expand/expand_util2.c \
			  src/utils/linked_list/linked_list_util.c \
			  src/utils/linked_list/free_element.c \
			  src/parsing/local_var/local_var_value.c \
			  src/parsing/local_var/local_var_name.c \
			  src/parsing/local_var/local_var_len.c \
			  src/parsing/local_var/local_var_list.c \
			  src/parsing/local_var/get_env_var.c \
			  src/build_in/utils.c \
			  src/parsing/delete_quote.c \
			  src/utils/ft_strjoin.c \
			  src/tree/redir.c \
			  src/tree/tree.c \
			  src/tree/exec/one_cmd/exec_one_cmd.c \
			  src/tree/exec/one_cmd/redir_cmd.c \
			  src/tree/exec/pipeline/exec_pipeline.c \
			  src/tree/exec/exec.c \
			  src/utils/ft_lstadd_front.c \
			  src/utils/ft_lstsize.c \
			  src/utils/ft_itoa.c \

OBJECTS     = ${SOURCES:.c=.o}
NAME        = Minishell
CC          = gcc
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
