#include <stdio.h>
#include "../include/hashfile.h"

int main() {
    HashFile* hf = createFile("teste");

    //Insere
    insertRegister(hf, 10, 100);
    insertRegister(hf, 20, 200);
    insertRegister(hf, 30, 300);
    insertRegister(hf, 40, 400); 

    //Busca
    int value;

    if (searchRegister(hf, 10, &value) == 0)
        printf("OK: 10 -> %d\n", value);
    else
        printf("ERRO: 10\n");

    if (searchRegister(hf, 30, &value) == 0)
        printf("OK: 30 -> %d\n", value);
    else
        printf("ERRO: 30\n");

    // Remoção
    removeRegister(hf, 20);

    if (searchRegister(hf, 20, &value) == -1)
        printf("OK: 20 removido\n");
    else
        printf("ERRO: remove 20\n");

    closeFile(hf);

    return 0;
}