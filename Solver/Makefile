# Makefile

# Compilers vars
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= sudoLoader.c sudoSolver.c test.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

test: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} test
