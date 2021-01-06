CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra
DEBUGFLAGS=-g -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Shortest example"
	./pr1.o data/test_simple.txt
	@echo " "
	@echo "Second shortest example"
	./pr1.o data/test_second.txt
	@echo " "
	@echo "Third example"
	./pr1.o data/test_three.txt
	@echo " "
	@echo "Fourth example"
	./pr1.o data/test_four.txt
	@echo " "
	@echo "Fifth example"
	./pr1.o data/test_five.txt

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
