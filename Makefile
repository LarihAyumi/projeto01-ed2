CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = src/hashfile.c src/svg.c src/geo.c src/quadra.c src/pessoa.c src/pm.c
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

	$(CC) $(CFLAGS) tests/test_pessoa.c src/pessoa.c $(INCLUDE) -o test_pessoa
	./test_pessoa

	$(CC) $(CFLAGS) tests/test_pm.c src/hashfile.c src/pessoa.c src/pm.c $(INCLUDE) -o test_pm
	./test_pm

persist:
	$(CC) $(CFLAGS) tests/test_persistencia.c src/hashfile.c $(INCLUDE) -o test_persist
	./test_persist

clean:
	del /Q prog.exe test_hash.exe test_svg.exe test_persist.exe test_pessoa.exe test_pm.exe *.hf *.hfc *.svg *.hfd *.dat 2>nul

clean-linux:
	rm -f prog test_hash test_svg test_persist *.hf *.hfc *.svg *.hfd *.dat

tudo: all run test