CC=g++
CFLAGS=-Wall -O3 -Wpedantic -Wextra
DEBUGFLAGS=-g -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	./pr2.o data/test.txt

test_debug: main
	./pr2.o data/test_debug.txt

run: main
	./pr2.o data/input.txt

debug:
	$(CC) $(DEBUGFLAGS) -o pr2.o pr2.cpp
	gdb pr2.o

.ONESHELL:
part:
	cd ..
	pwd
	cp -r pr2 pr2
	cd pr2
	sed -i "s/pr2/pr2/g" makefile
	rm pr2.o
	mv pr2.cpp pr2.cpp
