CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic  -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes -g -O0 -gdwarf-5 -fsanitize=address -DDEBUG

LIBS=-lallegro -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_dialog -lallegro_primitives -lallegro_image -lm

LIBS_DIR=-L/usr/lib64

INC_DIR=-I/usr/include
BIN=invaders_better

SRC=g_bullets.c\
    g_config.c\  g_enemies.c\  g_item.c  g_particles.c\  g_player.c\  g_render.c\  g_sound.c\  g_sprites.c\  main.c\  miniz.c\ g_score.c

OBJ=g_bullets.o  g_config.o  g_enemies.o  g_item.o  g_particles.o  g_player.o  g_render.o  g_sound.o  g_sprites.o  main.o  miniz.o g_score.o



all: $(OBJ)
	$(CC) $(CFLAGS)  $^  $(INC_DIR)  $(LIBS_DIR) $(LIBS) -lasan  -o $(BIN)


%.c%.o: $(SRC)
	$(CC) $(CFLAGS)  -c $^  $(INC_DIR)  $(LIBS_DIR) $(LIBS) 


clean:
	rm *.o $(BIN) $(BIN)-release 

copy:
	cp $(BIN) ../$(BIN)-x64



.PHONY: clean copy release
