CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra
DEBUGFLAGS=-g -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Third example"
	./pr2.o data/test_three.txt
	@echo " "
	@echo "Fourth example"
	./pr2.o data/test_four.txt
	@echo " "
	@echo "Fifth example"
	./pr2.o data/test_five.txt

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
