# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lm
LDLIBS =
EXEC = main

all: main
#all :
#	gcc -Wall -Wextra -Werror -lm -o main NeuralNetwork.c Maths.c Maths.h
SRC = Maths.c NeuralNetwork.c
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
