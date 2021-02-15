CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c11

sources = urzadzenia.c wypozyczenia.c zapytania.c main.c
objects = urzadzenia.o wypozyczenia.o zapytania.o main.o
dependencies = urzadzenia.h wypozyczenia.h zapytania.h
name = main

$(name): $(objects)
	$(CC) $(CFLAGS) -o $(name) $(objects) $(dependencies)

%.o.: %.c
	$(CC) $(CFLAGS) -c $< $(sources) $(dependencies)

clean:
	$(RM) $(name)
	$(RM) *.o