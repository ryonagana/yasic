CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic  -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes

LIBS=-lallegro -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_dialog -lallegro_primitives -lallegro_image

LIBS_DIR=-LD:\projetos\libs\liballegro-5.2.9-git-release\lib

INC_DIR=-ID:\projetos\libs\liballegro-5.2.9-git-release\include

all: main.o
	$(CC) $(CFLAGS) main.o $(INC_DIR)  $(LIBS_DIR) $(LIBS) -o invaders_better.exe


main.o: main.c
	$(CC) $(CFLAGS) main.c $(INC_DIR) $(LIBS_DIR) $(LIBS) -c
