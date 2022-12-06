# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = `pkg-config --cflags sdl2 SDL2_image` `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror
LDFLAGS = -lm
LDLIBS = `pkg-config --libs sdl2 SDL2_image` `pkg-config --libs gtk+-3.0`
EXEC = OCR

all: main

SRC = NeuralNetwork/GetImages.c NeuralNetwork/GuessFunction.c NeuralNetwork/main.c NeuralNetwork/TrainAndShow.c NeuralNetwork/Write.c NeuralNetwork/Maths.c NeuralNetwork/NeuralNetwork.c Image/Detection/lign_detection.c Image/Detection/sobel.c Image/pre_processing/filters.c Image/pre_processing/transformation.c Image/Display/display.c Image/Detection/square_detection.c Image/Perspective/correct_perspective.c Image/Perspective/matrix.c Image/Segmentation/split.c Image/Rotation_Resize/resize.c Image/Rotation_Resize/interpolation.c Image/Rotation_Resize/rotation.c Solver/sudoLoader.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main : ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main OCR
	${RM} test_* sudo_resolved [0-9]*.png
# END
