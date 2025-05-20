PROG	= philo

SRCS    = 	philo.c	\
        	action_routine_1.c	\
			action_routine_2.c	\
			check_begin.c	\
			check_in_routine_1.c \
			check_in_routine_2.c	\
			clean.c	\
			error.c	\
			init_1.c \
			init_2.c	\
			simulation.c	\
			thread.c	\
			time.c	\
			util.c
			

OBJS 	= ${SRCS:.c=.o}
MAIN	= philo.c 

HEADER	= -Iincludes

CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror  #-MMD -MP


%.o : %.c
		${CC} ${CFLAGS} ${HEADER} -c $< -o $@

all: 		${PROG}
${PROG}:	${OBJS}
					
					$(CC) ${OBJS} -o ${PROG}
					echo "\033[32mphilo Compiled!\n\e[0m"

clean:
					rm -f ${OBJS} ${OBJS_B}

fclean: 	clean
					rm -f ${PROG}

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re