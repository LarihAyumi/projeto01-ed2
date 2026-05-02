CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = src/hashfile.c src/svg.c src/geo.c
INCLUDE = -Iinclude

all:
	$(CC) $(CFLAGS) src/main.c $(SRC) $(INCLUDE) -o prog

run:
	./prog

test:
	$(CC) $(CFLAGS) tests/test_hashfile.c src/hashfile.c $(INCLUDE) -o test_hash
	./test_hash
	
	$(CC) $(CFLAGS) tests/test_svg.c src/svg.c $(INCLUDE) -o test_svg
	./test_svg

persist:
	$(CC) $(CFLAGS) tests/test_persistencia.c src/hashfile.c $(INCLUDE) -o test_persist
	./test_persist

clean:
	rm -f prog test_hash test_svg test_persist *.hf *.hfc *.svg *.hfd

tudo: all run test