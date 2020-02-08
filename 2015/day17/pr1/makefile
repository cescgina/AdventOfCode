CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra --std=c++17


main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be 4"
	./pr1.o data/test.txt

run: main
	./pr1.o data/input.txt
