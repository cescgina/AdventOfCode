CC=g++
CFLAGS=-Wall -O3 -Wpedantic -Wextra
DEBUGFLAGS=-g -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	./pr1.o data/test.txt

test_detail: main 
	@echo "Answer should be 3"
	./pr1.o data/test_detail.txt

test_debug: 
#	./pr1.o data/test_detail.txt
	$(CC) $(DEBUGFLAGS) -o pr1.o pr1.cpp
	gdb -x gdb_commands -ex run --args pr1.o data/test_detail.txt

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
