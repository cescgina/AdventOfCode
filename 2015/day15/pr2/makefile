CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -Icppitertools -std=c++17

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 57600000"
	./pr2.o data/test_1.txt
	@echo ""

run: main
	./pr2.o data/input.txt
