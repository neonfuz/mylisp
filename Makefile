bin/mylisp: *.c *.h
	mkdir -p bin
	gcc -o bin/mylisp *.c

clean:
	rm bin/*

.PHONY: clean
