CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = src/hashfile.c
INCLUDE = -Iinclude

all:
	$(CC) $(CFLAGS) src/main.c $(SRC) $(INCLUDE) -o prog

test:
	$(CC) $(CFLAGS) tests/test_hashfile.c $(SRC) $(INCLUDE) -o test_hash
	./test_hash

persist:
	$(CC) $(CFLAGS) tests/test_persistencia.c $(SRC) $(INCLUDE) -o test_persist
	./test_persist

clean:
	rm -f prog test_hash test_persist *.hf *.hfc