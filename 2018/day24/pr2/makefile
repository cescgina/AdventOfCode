CC=g++
CFLAGS=-Wall -O3 -Wpedantic -Wextra -lboost_regex
DEBUGFLAGS=-g -Wpedantic -Wextra -lboost_regex

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	./pr2.o data/test.txt

test_detail: main 
	./pr2.o data/test_detail.txt

test_debug: 
#	./pr2.o data/test_detail.txt
	$(CC) $(DEBUGFLAGS) -o pr2.o pr2.cpp
	gdb -x gdb_commands -ex run --args pr2.o data/test_detail.txt

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
