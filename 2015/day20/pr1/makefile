CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be 6"
	./pr1.o data/test.txt

run: main
	./pr1.o data/input.txt

part:
	sed -i "s/pr1/pr2/g" makefile
	rm pr1.o
	mv pr1.cpp pr2.cpp
