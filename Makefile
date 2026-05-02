CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = src/hashfile.c src/svg.c src/geo.c src/quadra.c src/pessoa.c src/pm.c
UNITY = Unity/unity.c
INCLUDE = -Iinclude -IUnity

all:
	$(CC) $(CFLAGS) src/main.c $(SRC) $(INCLUDE) -o prog

run:
	./prog

test:
	$(CC) $(CFLAGS) tests/test_hashfile.c src/hashfile.c $(UNITY) $(INCLUDE) -o test_hash
	./test_hash
	
	$(CC) $(CFLAGS) tests/test_svg.c src/svg.c $(UNITY) $(INCLUDE) -o test_svg
	./test_svg

	$(CC) $(CFLAGS) tests/test_quadra.c src/quadra.c $(UNITY) $(INCLUDE) -o test_quadra
	./test_quadra

	$(CC) $(CFLAGS) tests/test_geo.c src/geo.c src/svg.c src/quadra.c src/hashfile.c $(UNITY) $(INCLUDE) -o test_geo
	./test_geo

	$(CC) $(CFLAGS) tests/test_pessoa.c src/pessoa.c $(UNITY) $(INCLUDE) -o test_pessoa
	./test_pessoa

	$(CC) $(CFLAGS) tests/test_pm.c src/hashfile.c src/pessoa.c src/pm.c $(UNITY) $(INCLUDE) -o test_pm
	./test_pm


clean:
	del /Q prog.exe test_hash.exe test_svg.exe test_pessoa.exe test_pm.exe test_quadra.exe test_geo.exe *.hf *.hfc *.svg *.hfd *.dat 2>nul

clean-linux:
	rm -f prog test_hash test_svg test_pessoa.exe test_pm.exe test_geo.exe *.hf *.hfc *.svg *.hfd *.dat

tudo: all run test