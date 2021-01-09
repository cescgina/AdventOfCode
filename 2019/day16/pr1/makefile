CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra
DEBUGFLAGS=-g -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "First test"
	./pr1.o data/test.txt
	@echo ""
	@echo "Second test"
	./pr1.o data/test_2.txt
	@echo ""
	@echo "Third test"
	./pr1.o data/test_3.txt
	@echo ""
	@echo "Fourth test"
	./pr1.o data/test_4.txt

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
