bin/mylisp: main.c
	mkdir -p bin
	gcc -o bin/mylisp main.c

clean:
	rm bin/*

.PHONY: clean
