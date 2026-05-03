#include <stdio.h>
#include "../include/hashfile.h"

int main() {
    // cria e insere
    HashFile* hf = createFile("teste2");

    insertRegister(hf, 1, 111);
    insertRegister(hf, 2, 222);

    closeFile(hf);

    // reabre
    hf = openFile("teste2");

    int value;

    if (searchRegister(hf, 1, &value) == 0)
        printf("OK persistencia: 1 -> %d\n", value);
    else
        printf("ERRO persistencia\n");

    closeFile(hf);

    return 0;
}