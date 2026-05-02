#include "../include/hashfile.h"
#include <stdio.h>
#include <string.h>

int main() {
    HashFile* hf = createFile("teste", sizeof(int));

    int v1 = 100, v2 = 200, v3 = 300, v4 = 400;

    insertRegister(hf, "10", &v1);
    insertRegister(hf, "20", &v2);
    insertRegister(hf, "30", &v3);
    insertRegister(hf, "40", &v4);

    int value;

    if (searchRegister(hf, "10", &value) == 0)
        printf("OK: 10 -> %d\n", value);

    if (searchRegister(hf, "30", &value) == 0)
        printf("OK: 30 -> %d\n", value);

    removeRegister(hf, "20");

    if (searchRegister(hf, "20", &value) == -1)
        printf("OK: 20 removido\n");

    closeFile(hf);

    return 0;
}