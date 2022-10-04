# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -lm
LDFLAGS =
LDLIBS =

all: main

SRC = reseau_test.c Maths.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main : ${OBJ}

-include ${DEP} 

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END
