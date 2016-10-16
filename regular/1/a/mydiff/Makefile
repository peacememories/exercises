CC=gcc
DEPS = 
OBJ = mydiff.c
CFLAGS = -Wall -Werror -pedantic -std=c99

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mydiff: $(OBJ) $(DEPS)
	gcc -o $@ $^ $(CFLAGS)