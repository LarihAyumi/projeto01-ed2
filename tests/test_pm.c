#include "../include/hashfile.h"
#include "../include/pessoa.h"
#include "../include/pm.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    HashFile* pessoasHash = createFile("teste_pessoas", getPessoaSize());

    if (pessoasHash == NULL) {
        printf("ERRO: hash de pessoas nao criado\n");
        return 1;
    }

    processPM("tests/pessoas.pm", pessoasHash);

    Pessoa* p = malloc(getPessoaSize());

    if (searchRegister(pessoasHash, "000.000.001-91", p) == 0) {
        printf("OK: pessoa encontrada no hash\n");

        if (pessoaTemMoradia(p)) {
            printf("OK: moradia encontrada no hash\n");
        } else {
            printf("ERRO: moradia nao foi gravada\n");
            free(p);
            closeFile(pessoasHash);
            return 1;
    }
}

    free(p);
    closeFile(pessoasHash);

    return 0;
}