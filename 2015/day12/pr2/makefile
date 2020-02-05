CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 6"
	./pr2.o data/test_1.txt
	@echo ""
	@echo "Answer should be 4"
	./pr2.o data/test_2.txt
	@echo ""
	@echo "Answer should be 0"
	./pr2.o data/test_3.txt
	@echo ""
	@echo "Answer should be 6"
	./pr2.o data/test_4.txt
	@echo ""

run: main
	./pr2.o data/input.txt

debug: 
	$(CC) -g -o pr2.o pr2.cpp
