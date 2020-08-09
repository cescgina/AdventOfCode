CC=g++
CFLAGS=-Wall -O2 -Wpedantic -Wextra

main:
	$(CC) $(CFLAGS) -o pr2.o pr2.cpp

run: main
	./pr2.o data/input.txt

test: main
	@echo "Answer should be 9"
	./pr2.o data/test.txt
	@echo "Answer should be 241920"
	./pr2.o data/test_1.txt
	@echo "Answer should be 20"
	./pr2.o data/test_2.txt
	@echo "Answer should be 445"
	./pr2.o data/test_3.txt

.ONESHELL:
part:
	cd ..
	pwd
	cp -r pr2 pr2
	cd pr2
	sed -i "s/pr2/pr2/g" makefile
	rm pr2.o
	mv pr2.cpp pr2.cpp
