.PHONY: all clean run

all: test

test: BST.cpp
	g++ BST.cpp -o test -L. -lRBTreeTest
run: all
	./test
clean: 
	rm -rf *.o test
