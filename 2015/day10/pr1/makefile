CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be 11"
	./pr1.o data/test_1.txt
	@echo ""
	@echo "Answer should be 21"
	./pr1.o data/test_2.txt
	@echo ""
	@echo "Answer should be 1211"
	./pr1.o data/test_3.txt
	@echo ""
	@echo "Answer should be 111221"
	./pr1.o data/test_4.txt
	@echo ""
	@echo "Answer should be 312211"
	./pr1.o data/test_5.txt
	@echo ""

run: main
	./pr1.o data/input.txt
