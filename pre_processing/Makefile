# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: filters

SRC = filters.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

filters: filters.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
