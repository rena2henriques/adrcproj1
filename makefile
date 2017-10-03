#
#  File name: makefile
#
#  Author: 2017 Renato Henriques e Paulo Eusebio
#
#  Usage:
#     1) To generate the executable file for the program client,
#       run
#         $ make
#
#     2) To clean all object files and the executable file of
#        client, run
#         $ make clean
#

#  Compiler
CC = gcc

#  Compiler Flags
CFLAGS = -Wall -c -g

#  Sources
SOURCES = utils.c prefixTrees.c main.c

#  Objects
OBJECTS = utils.o prefixTrees.o main.o

prefixTrees: $(OBJECTS)
	gcc -o $@ $(OBJECTS)


utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

prefixTrees.o: prefixTrees.c prefixTrees.h
	$(CC) $(CFLAGS) prefixTrees.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean::
	rm -f *.o core a.out prefixTrees *~

depend::
	makedepend $(SOURCES)
# DO NOT DELETE
