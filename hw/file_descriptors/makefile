CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

all: wc head tail

obj/utils.o: src/utils.c src/utils.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/utils.c -o obj/utils.o

obj/wc.o: src/wc/wc.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/wc/wc.c -o obj/wc.o

wc: obj/wc.o obj/utils.o
	@mkdir -p obj
	$(CC) $(CFLAGS) -o wc src/wc/main.c obj/wc.o obj/utils.o

head: src/head/head.h obj/utils.o
	@mkdir -p obj
	$(CC) $(CFLAGS) -o head src/head/main.c obj/utils.o

obj/tail.o: src/tail/tail.c src/tail/tail.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/tail/tail.c -o obj/tail.o

tail: obj/tail.o obj/utils.o
	@mkdir -p obj
	$(CC) $(CFLAGS) -o tail src/tail/main.c obj/tail.o obj/utils.o

clean:
	find . -type f -name '*.o' -exec rm {} +
	find . -type f -name '*.dSYM' -exec rm -r {} +
	find . -type f -name '*.out' -exec rm {} +
	find . -type f -name '*.exe' -exec rm {} +
	rm wc head tail
	rmdir obj