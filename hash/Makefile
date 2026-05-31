.PHONY: all clean run

all: test

test: HashTable.cpp
	g++ HashTable.cpp -L. -lHashTest -o test
run: all
	./test
clean:
	rm -rf *.o test
