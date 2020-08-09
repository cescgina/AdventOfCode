CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

debug:
	$(CC) $(CFLAGS) -g -o pr1.o pr1.cpp
	gdb pr1.o

run: main
	./pr1.o data/input.txt

test: main
	./pr1.o data/test.txt

.ONESHELL:
part:
	cd ..
	pwd
	cp -r pr1 pr2
	cd pr2
	sed -i "s/pr1/pr2/g" makefile
	rm pr1.o
	mv pr1.cpp pr2.cpp
