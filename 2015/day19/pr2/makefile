CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 6"
	./pr2.o data/test.txt
	@echo ""
	@echo "Answer should be 3"
	./pr2.o data/test_1.txt

run: main
	./pr2.o data/input.txt

part:
	sed -i "s/pr2/pr2/g" makefile
	rm pr2.o
	mv pr2.cpp pr2.cpp
