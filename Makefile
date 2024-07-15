CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

micro: micro.c
	$(CC) $(CFLAGS) -o micro micro.c
