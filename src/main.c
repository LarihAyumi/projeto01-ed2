#include "../include/geo.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include <stdio.h>

int main() {

    HashFile* quadras = createFile("quadras", getQuadraSize());

    if (!quadras) {
        printf("Erro ao criar hashfile\n");
        return 1;
    }

    processGeo("tests/cidade.geo", "saida.svg", quadras);

    closeFile(quadras);

    return 0;
}