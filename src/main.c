#include "../include/geo.h"
#include "../include/pm.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include "../include/pessoa.h"
#include <stdio.h>

int main() {

    HashFile* quadras = createFile("quadras", getQuadraSize());
    HashFile* pessoas = createFile("pessoas", getPessoaSize());

    if (!quadras || !pessoas) {
        printf("Erro ao criar hashfile\n");
        return 1;
    }

    processGeo("tests/cidade.geo", "saida.svg", quadras);
    processPM("pessoas.pm", pessoas);

    closeFile(quadras);
    closeFile(pessoas);

    return 0;
}