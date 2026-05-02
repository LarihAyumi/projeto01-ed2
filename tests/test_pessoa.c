#include "../include/pessoa.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Pessoa* p = createPessoa(
        "000.000.001-91",
        "Taisa_Ana",
        "Lombardi_De_Angelis",
        'F',
        "26/03/2023"
    );

    if (p != NULL) {
        printf("OK: pessoa criada\n");
    } else {
        printf("ERRO: pessoa nao criada\n");
        return 1;
    }

    setMoradia(p, "cep1", 'S', 45, "apto101");
    printf("OK: moradia definida\n");

    removeMoradia(p);
    printf("OK: moradia removida\n");

    free(p);

    return 0;
}