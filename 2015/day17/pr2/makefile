CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra --std=c++17


main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 3"
	./pr2.o data/test.txt

run: main
	./pr2.o data/input.txt
