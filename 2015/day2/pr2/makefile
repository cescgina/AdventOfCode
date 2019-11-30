CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	@$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 34"
	./pr2.o data/test_1.txt
	@echo ""
	@echo "Answer should be 14"
	./pr2.o data/test_2.txt

run: main
	./pr2.o data/input.txt
