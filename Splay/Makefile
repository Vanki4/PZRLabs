.PHONY: all clean run

all: test

test: main.cpp
	g++ main.cpp -o test

run: all
	./test

clean:
	rm -rf *.o test
