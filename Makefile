#* ************************************************************************** *#
#*                                                                            *#
#*                                                        :::      ::::::::   *#
#*   units.c                                            :+:      :+:    :+:   *#
#*                                                    +:+ +:+         +:+     *#
#*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        *#
#*                                                +#+#+#+#+#+   +#+           *#
#*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             *#
#*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       *#
#*                                                                            *#
#* ************************************************************************** *#

NAME = philo

LIST_INCL = ./src/

LIST_FUNC = main.c dead_philo.c init.c parse.c philosopher.c time.c units.c

HDR_INCL = ./include/

HDR = philosopher.h

HDR_PHILO = ${addprefix ${HDR_INCL}, ${HDR}}

CC = gcc -pthread

FLAGS = -Wall -Werror -Wextra

LIST = ${addprefix ${LIST_INCL}, ${LIST_FUNC}}

OBJ1 = $(LIST:.c=.o)

${NAME}:	${OBJ1}
				${CC} ${FLAGS} $^ -o $@

%.o : %.c ${HDR_PHILO} Makefile
	${CC} ${FLAGS} -c $< -o $@

all: ${NAME}

clean:
	rm -f ${OBJ1}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONNY : all bonus clean fclean re