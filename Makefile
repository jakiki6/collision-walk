all: main

main: main.c
	gcc -o $@ $< -O3 -lbsd

test: main
	./main

clean:
	rm -f main

.PHONY: all test clean
