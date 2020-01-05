CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra -lssl -lcrypto

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

test: main
	@echo "Answer should be 1"
	./pr2.o data/test_1.txt
	@echo ""
	@echo "Answer should be 2000000"
	./pr2.o data/test_2.txt
	@echo ""
	@echo "Answer should be 1000000"
	./pr2.o data/test_3.txt
	@echo ""
	@echo "Answer should be 999000"
	./pr2.o data/test_4.txt

run: main
	./pr2.o data/input.txt
