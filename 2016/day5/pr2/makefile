CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	./pr2.o data/test.txt

run: main
	./pr2.o data/input.txt

.ONESHELL:
part:
	cd ..
	pwd
	cp -r pr2 pr2
	cd pr2
	sed -i "s/pr2/pr2/g" makefile
	rm pr2.o
	mv pr2.cpp pr2.cpp
