#include "../include/qry.h"
#include "../include/pessoa.h"
#include <string.h>
#include <stdlib.h>

void processQry( const char* qryPath, HashFile* pessoasHash, HashFile* quadrasHash, FILE* txt, FILE* svg) {
    (void) quadrasHash;
    (void) svg;

    FILE* qry = fopen(qryPath, "r");
    if (!qry) return;

    char comando[10];

    while (fscanf(qry, "%s", comando) != EOF) {

        if (strcmp(comando, "rq") == 0) {

        }

        else if (strcmp(comando, "pq") == 0) {

        }

        else if (strcmp(comando, "censo") == 0) {

        }

        else if (strcmp(comando, "h?") == 0) {
            char cpf[20];
            fscanf(qry, "%s", cpf);

            Pessoa* p = malloc(getPessoaSize());

            if (searchRegister(pessoasHash, cpf, p) == 0) {
                fprintf(txt, "%s %s\n", getNome(p), getSobrenome(p));
            }

            free(p);
        }

        else if (strcmp(comando, "nasc") == 0) {
            char cpf[20], nome[50], sobrenome[50], nasc[11];
            char sexo;

            fscanf(qry, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nasc);
            Pessoa* p = createPessoa(cpf, nome, sobrenome, sexo, nasc);
            
            insertRegister(pessoasHash, cpf, p);

            free(p);
        }

        else if (strcmp(comando, "rip") == 0) {

        }
        
        else if (strcmp(comando, "mud") == 0) {

        }

        else if (strcmp(comando, "dspj") == 0) {

        }
    }

    fclose(qry);
}