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

    fprintf(qry, "h? 800.577.369-28\n");

    fclose(qry);
}

void tearDown(void) {
    remove("teste.qry");
    remove("teste.txt");
    remove("teste.svg");
    remove("teste_qry_hash.hf");
    remove("teste_qry_hash.hfc");
    remove("saida.hfd");
}

void testH(void) {
    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa( "800.577.369-28", "Larissa", "Costa", 'F', "26/08/2004");

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

    TEST_ASSERT_NOT_NULL(fgets(buffer, sizeof(buffer), txt));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Larissa"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Costa"));

    fclose(txt);
    closeFile(pessoasHash);
}

void testNasc(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "nasc 123 Teste Larissa F 26/08/2004\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    FILE* txt = fopen("teste.txt", "w");
    FILE* svg = fopen("teste.svg", "w");

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    // pra verificar se a pessoa foi inserida
    Pessoa* p = malloc(getPessoaSize());

    int res = searchRegister(pessoasHash, "123", p);

    TEST_ASSERT_EQUAL_INT(0, res);
    TEST_ASSERT_EQUAL_STRING("Teste", getNome(p));

    free(p);
    closeFile(pessoasHash);
}


void testRip(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "rip 800.577.369-28\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa( "800.577.369-28", "Larissa", "Costa", 'F', "26/08/2004");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("teste.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    Pessoa* removida = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(removida);

    TEST_ASSERT_EQUAL_INT(-1, searchRegister(pessoasHash, "800.577.369-28", removida));

    free(removida);

    txt = fopen("teste.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrou = 1;
            break;
        }
    }

    TEST_ASSERT_TRUE(encontrou);

    fclose(txt);
    closeFile(pessoasHash);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testH);
    RUN_TEST(testNasc);
    RUN_TEST(testRip);

    return UNITY_END();
}