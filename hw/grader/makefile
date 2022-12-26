CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

all: static dynamic

obj/utils.o: src/utils/file.c src/utils/file.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/utils/file.c -o obj/utils.o

obj/dynamic.o:
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/dynamic.c -o obj/dynamic.o

obj/static.o:
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/static.c -o obj/static.o

obj/main.o: src/main.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

dynamic: obj/utils.o obj/dynamic.o obj/main.o
	$(CC) $(CFLAGS) obj/utils.o obj/dynamic.o obj/main.o -o dynamic.out

static: obj/utils.o obj/static.o obj/main.o
	$(CC) $(CFLAGS) -static obj/utils.o obj/static.o obj/main.o -o static.out

clean:
	find . -type f -name '*.o' -exec rm {} +
	find . -type f -name '*.dSYM' -exec rm -r {} +
	find . -type f -name '*.out' -exec rm {} +
	find . -type f -name '*.exe' -exec rm {} +
	rm -rf obj