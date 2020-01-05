CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

run: main
	./pr2.o data/input.txt
