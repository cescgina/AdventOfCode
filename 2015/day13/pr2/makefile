CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 330"
	./pr2.o data/test_1.txt
	@echo ""

run: main
	./pr2.o data/input.txt
