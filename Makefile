# Copyright Cretu Mihnea Tudor 315CAa 2022 - 2023
CFLAGS= -Wall -Wextra -std=c99

all : vma

build : main.o vma.o
	gcc $(CFLAGS) -o vma main.o vma.o

vma.o: vma.h vma.c
	gcc $(CFLAGS) -c -g vma.c

main.o: main.c vma.h
	gcc $(CFLAGS) -c main.c

run_vma : vma
	./vma

clean:
	rm -f *.o vma
	
pack:
	zip -FSr 315CA_CretuMihneaTudor_Tema1.zip README.md Makefile *.c *.h

.PHONY: pack clean