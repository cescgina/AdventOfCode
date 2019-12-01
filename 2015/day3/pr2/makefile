CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	@$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 3"
	./pr2.o data/test_1.txt
	@echo ""
	@echo "Answer should be 3"
	./pr2.o data/test_2.txt
	@echo ""
	@echo "Answer should be 11"
	./pr2.o data/test_3.txt

run: main
	./pr2.o data/input.txt
