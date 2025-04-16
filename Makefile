PROG	= push_swap

SRCS    = 	push_swap.c \
        	push_swap_2.c \
			push.c \
			swap.c \
			rotate.c \
			rev_rotate.c \
			utils_algo_1.c \
			utils_algo_2.c \
			utils_algo_3.c \
          	utils_arg_1.c \
			utils_arg_2.c \
			utils_list_1.c \
			utils_list_2.c \
			utils_list_3.c 

OBJS 	= ${SRCS:.c=.o}
MAIN	= push_swap.c 

HEADER	= -Iincludes

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror -g #-MMD -MP


%.o : %.c
		${CC} ${CFLAGS} ${HEADER} -c $< -o $@

all: 		${PROG}
${PROG}:	${OBJS}
					echo "\033[36m\n"
					make  -C ./libft
					echo "\nLibft Compiled!"
					echo "\033[35m"
					$(CC) ${OBJS} -L./libft -lft -o ${PROG}
					echo "\033[36mLibft Compiled!\e[0m"
					echo "\033[32mPush_swap Compiled!\n\e[0m"



clean:
					make clean -C ./libft
					rm -f ${OBJS} ${OBJS_B}

fclean: 	clean
					make fclean -C ./libft
					rm -f ${PROG}

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re