bin/mylisp: *.c *.h
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/mylisp *.c

clean:
	rm bin/*

.PHONY: clean
