# Variables set after target match
# $@ full target
# $* target minus suffix
# $< first prerequisite
# $^ All prerequisities
# -g2 is default, -g3 doesnt work with all debuggers

LIBS = libkputils.c
OBJS = $(LIBS:.c=.o)

CMAIN=main.c
PROGRAM=main
CPP=g++
CC=gcc
CLANG=clang
OBJECTS=
CFLAGS= -ggdb3 -Wall -O0 -Wextra -pipe --std=c11 -Wpedantic -Wfloat-equal -Wnull-dereference -Wshadow -Wimplicit
CFLAGSPC= $(shell pkg-config --cflags --libs glib-2.0)

gcc: $(CMAIN) $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $^

clang: $(CMAIN) $(OBJS)
	$(CLANG) $(CFLAGS) -o $(PROGRAM) $^

libs: $(LIBS)
	$(CC) -c -Wall -Werror -fpic libkputils.c -o libkputils.o 
	gcc -shared -o libkputils.so libkputils.o
	ar -cvq ./libkputils.a *.o

clanglibs: libkputils.c
	$(CLANG) -c -Wall -Werror -fpic libkputils.c -o libkputils.o 
	gcc -shared -o libkputils.so libkputils.o
	ar -cvq ./libkputils.a *.o

clean:
	rm *.o
