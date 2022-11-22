# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = `pkg-config --libs sdl2 SDL2_image` -Wall -Wextra -Werror
LDFLAGS = -lm
LDLIBS = `pkg-config --libs sdl2 SDL2_image`
EXEC = main

all: main
#all :
#	gcc -Wall -Wextra -Werror -lm -o main NeuralNetwork.c Maths.c Maths.h
SRC = NeuralNetwork/GetImages.c NeuralNetwork/GuessFunction.c NeuralNetwork/main.c NeuralNetwork/TrainAndShow.c NeuralNetwork/Write.c NeuralNetwork/Maths.c NeuralNetwork/NeuralNetwork.c
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
