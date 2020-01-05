CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be 12"
	./pr1.o data/test_1.txt
	@echo ""

run: main
	./pr1.o data/input.txt
