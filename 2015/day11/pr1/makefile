CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be abcdffaa"
	./pr1.o data/test_1.txt
	@echo ""
	@echo "Answer should be ghjaabcc"
	./pr1.o data/test_2.txt
	@echo ""

run: main
	./pr1.o data/input.txt
