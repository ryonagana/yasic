CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic  -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes -g -O0 -gdwarf-2 -fsanitize=address

LIBS=-lallegro -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_dialog -lallegro_primitives -lallegro_image -lm -lasan

LIBS_DIR=-L/usr/lib64

INC_DIR=-I/usr/include
BIN=invaders_better


all: miniz.o main.o
	$(CC) $(CFLAGS) $^  $(INC_DIR)  $(LIBS_DIR) $(LIBS) -o $(BIN)


miniz.o: miniz.c
	$(CC) $(CFLAGS) -c  $^ $(INC_DIR) $(LIBS_DIR) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) $^ $(INC_DIR) $(LIBS_DIR) $(LIBS) -c

clean:
	rm *.o $(BIN) 

copy:
	cp $(BIN) ../$(BIN)-x64



.PHONY: clean copy
