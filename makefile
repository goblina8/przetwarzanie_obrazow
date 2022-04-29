CC = gcc
CFLAGS = -Wall -pedantic

Laby5: main.o modul.o
	$(CC) -o Lena2 main.o modul.o -lm
main.o: main.c modul.h
	$(CC) -c $(CFLAGS) main.c
modul.o: modul.c modul.h
	$(CC) -c modul.c

