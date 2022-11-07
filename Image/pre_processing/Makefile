# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: main

SRC = lign_detection.c sobel.c filters.c main.c transformation.c display.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main : ${OBJ}

-include ${DEP} 

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main
	${RM} test_*.png

# END
