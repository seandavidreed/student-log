CC = gcc
CFLAGS = -g -Wall

all: studentlog

logtools.o: logtools.h logtools.c
	$(CC) $(CFLAGS) -c logtools.c

queries.o: queries.h queries.c # queries can be removed - used to seed studentlog4.0 database
	$(CC) $(CFLAGS) -c queries.c

studentlog: logtools.o queries.o main.c
	$(CC) $(CFLAGS) -o studentlog main.c logtools.c queries.c

clean:
	rm -f *.o studentlog
