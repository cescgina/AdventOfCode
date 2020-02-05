CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr1.o pr1.cpp

test: main
	@echo "Answer should be 6"
	./pr1.o data/test_1.txt
	@echo ""
	@echo "Answer should be 6"
	./pr1.o data/test_2.txt
	@echo ""
	@echo "Answer should be 3"
	./pr1.o data/test_3.txt
	@echo ""
	@echo "Answer should be 3"
	./pr1.o data/test_4.txt
	@echo ""
	@echo "Answer should be 0"
	./pr1.o data/test_5.txt
	@echo ""
	@echo "Answer should be 0"
	./pr1.o data/test_6.txt
	@echo ""
	@echo "Answer should be 0"
	./pr1.o data/test_7.txt
	@echo ""
	@echo "Answer should be 0"
	./pr1.o data/test_8.txt
	@echo ""

run: main
	./pr1.o data/input.txt

debug: 
	$(CC) -g -o pr1.o pr1.cpp
