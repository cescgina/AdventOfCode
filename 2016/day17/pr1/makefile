CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto
DEBUGFLAGS=-g -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test_all: main
	@echo "DDRRRD"
	./pr1.o data/test.txt
	@echo ""
	@echo "DDUDRLRRUDRD"
	./pr1.o data/test_2.txt
	@echo ""
	@echo "DRURDRUDDLLDLUURRDULRLDUUDDDRR"
	./pr1.o data/test_3.txt

test: main
	@echo "DDRRRD"
	./pr1.o data/test.txt

test2: main
	@echo "DDUDRLRRUDRD"
	./pr1.o data/test_2.txt

test3: main
	@echo "DRURDRUDDLLDLUURRDULRLDUUDDDRR"
	./pr1.o data/test_3.txt

run: main
	./pr1.o data/input.txt

debug:
	$(CC) $(DEBUGFLAGS) -o pr1.o pr1.cpp
	gdb pr1.o

.ONESHELL:
part:
	cd ..
	pwd
	cp -r pr1 pr2
	cd pr2
	sed -i "s/pr1/pr2/g" makefile
	rm pr1.o
	mv pr1.cpp pr2.cpp
