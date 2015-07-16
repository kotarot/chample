PROGRAM = chample
OBJS = main.o coordcube.o cubiecube.o facecube.o mytools.o search.o
CC = gcc
CFLAGS = -Wall -O3

.SUFFIXES: .c .o

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
