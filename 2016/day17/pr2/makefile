CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto
DEBUGFLAGS=-g -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test_all: main
	@echo "370"
	./pr2.o data/test.txt
	@echo ""
	@echo "492"
	./pr2.o data/test_2.txt
	@echo ""
	@echo "830"
	./pr2.o data/test_3.txt

test: main
	@echo "370"
	./pr2.o data/test.txt

test2: main
	@echo "492"
	./pr2.o data/test_2.txt

test3: main
	@echo "830"
	./pr2.o data/test_3.txt

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
