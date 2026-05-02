#include "../Unity/unity.h"
#include "../include/qry.h"
#include "../include/hashfile.h"
#include "../include/pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "h? 000.000.001-91\n");

    fclose(qry);
}

void tearDown(void) {
    remove("teste.qry");
    remove("teste.txt");
    remove("teste_qry_hash.hf");
    remove("teste_qry_hash.hfc");
}

void testHQuery(void) {
    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa(
        "000.000.001-91",
        "Teste",
        "Pessoa",
        'F',
        "01/01/2000"
    );

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("teste.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("teste.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    fgets(buffer, sizeof(buffer), txt);

    TEST_ASSERT_NOT_NULL(strstr(buffer, "Teste"));

    fclose(txt);
    closeFile(pessoasHash);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testHQuery);

    return UNITY_END();
}