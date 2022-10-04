CC = gcc
CFLAGS = -g

all: studentlog

sqlite3.o: local_headers/sqlite3.h local_headers/sqlite3.c
	$(CC) $(CFLAGS) -c local_headers/sqlite3.c

in_out.o: local_headers/in_out.h local_headers/in_out.c
	$(CC) $(CFLAGS) -c local_headers/in_out.c

tools.o: local_headers/tools.h local_headers/tools.c
	$(CC) $(CFLAGS) -c local_headers/tools.c

studentlog: sqlite3.o in_out.o tools.o main.c
	$(CC) $(CFLAGS) -o studentlog main.c local_headers/sqlite3.c local_headers/in_out.c local_headers/tools.c

	rm in_out.o tools.o
