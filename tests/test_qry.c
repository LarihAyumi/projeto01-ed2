#include "../Unity/unity.h"
#include "../include/qry.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
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
    remove("teste_qry_hash.hf");
    remove("teste_qry_hash.hfc");
    remove("saida.hfd");

    remove("testeRQ.txt");
    remove("testeRQ.svg");

    remove("testePQ.txt");
    remove("testePQ.svg");

    remove("testeCENSO.txt");
    remove("testeCENSO.svg");

    remove("testeDSPJ.txt");
    remove("testeDSPJ.svg");

    remove("testeH.txt");
    remove("testeH.svg");

    remove("testeMUD.txt");
    remove("testeMUD.svg");

    remove("testeNasc.txt");
    remove("testeNasc.svg");

    remove("testeRIP.txt");
    remove("testeRIP.svg");
}

void testRq(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "rq cep1\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());

    TEST_ASSERT_NOT_NULL(pessoasHash);
    TEST_ASSERT_NOT_NULL(quadrasHash);


    Quadra* q = createQuadra("cep1", 0, 0, 100, 100);
    insertRegister(quadrasHash, "cep1", q);
    free(q);

 
    
    Pessoa* p = createPessoa("123", "Larissa", "Costa", 'F', "26/08/2004");
    setMoradia(p, "cep1", 'S', 10, "casa");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeRQ.txt", "w");
    FILE* svg = fopen("testeRQ.svg", "w");

    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg);

    fclose(txt);
    fclose(svg);

    Pessoa* res = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(res);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "123", res));
    TEST_ASSERT_FALSE(pessoaTemMoradia(res));

    free(res);

    txt = fopen("testeRQ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), txt)) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrou = 1;
        }
    }

    TEST_ASSERT_TRUE(encontrou);

    fclose(txt);

    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testPq(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "pq cep1\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());

    TEST_ASSERT_NOT_NULL(pessoasHash);
    TEST_ASSERT_NOT_NULL(quadrasHash);

    //quadra
    Quadra* q = createQuadra("cep1", 0, 0, 100, 100);
    insertRegister(quadrasHash, "cep1", q);
    free(q);

    //pessoas
    Pessoa* p1 = createPessoa("1", "A", "A", 'F', "01");
    setMoradia(p1, "cep1", 'N', 10, "casa");

    Pessoa* p2 = createPessoa("2", "B", "B", 'F', "01");
    setMoradia(p2, "cep1", 'S', 10, "casa");

    Pessoa* p3 = createPessoa("3", "C", "C", 'F', "01");
    setMoradia(p3, "cep1", 'S', 10, "casa");

    insertRegister(pessoasHash, "1", p1);
    insertRegister(pessoasHash, "2", p2);
    insertRegister(pessoasHash, "3", p3);

    free(p1); free(p2); free(p3);

    FILE* txt = fopen("testePQ.txt", "w");
    FILE* svg = fopen("testePQ.svg", "w");

    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("testePQ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrouTotal = 0;

    while (fgets(buffer, sizeof(buffer), txt)) {
        if (strstr(buffer, "Total: 3") != NULL) {
            encontrouTotal = 1;
        }
    }

    TEST_ASSERT_TRUE(encontrouTotal);

    fclose(txt);

    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testCenso(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);
    fprintf(qry, "censo\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL( pessoasHash);

    Pessoa* p1 = createPessoa("1", "Larissa", "Costa", 'F', "26/08/2004");
    setMoradia(p1, "cep1", 'N', 10, "apartamento");
    Pessoa* p2 = createPessoa("2", "Bruna", "Yokoshiro", 'F', "23/09/2005");
    setMoradia(p2, "cep1", 'S', 20, "casa");
    Pessoa* p3 = createPessoa("3", "Victor", "Hirota", 'M', "13/01/2005");

    insertRegister(pessoasHash, "1",p1);
    insertRegister(pessoasHash, "2",p2);
    insertRegister(pessoasHash, "3",p3);

    free(p1);
    free(p2);
    free(p3);

    FILE* txt = fopen("testeCENSO.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeCENSO.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeCENSO.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int achouHabitantes = 0;
    int achouMoradores = 0;
    int achouSemTetos = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Habitantes: 3") != NULL) achouHabitantes = 1;
        if (strstr(buffer, "Moradores: 2") != NULL) achouMoradores = 1;
        if (strstr(buffer, "Sem-tetos: 1") != NULL) achouSemTetos = 1;
    }

    TEST_ASSERT_TRUE(achouHabitantes);
    TEST_ASSERT_TRUE(achouMoradores);
    TEST_ASSERT_TRUE(achouSemTetos);

    fclose(txt);
    closeFile(pessoasHash);
}

void testH(void) {
    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa( "800.577.369-28", "Larissa", "Costa", 'F', "26/08/2004");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeH.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeH.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeH.txt", "r");
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

    fprintf(qry, "nasc 123 Larissa Costa F 26/08/2004\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    FILE* txt = fopen("testeNasc.txt", "w");
    FILE* svg = fopen("testeNasc.svg", "w");

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    // pra verificar se a pessoa foi inserida
    Pessoa* p = malloc(getPessoaSize());

    int res = searchRegister(pessoasHash, "123", p);

    TEST_ASSERT_EQUAL_INT(0, res);
    TEST_ASSERT_EQUAL_STRING("Larissa", getNome(p));
    TEST_ASSERT_EQUAL_STRING("26/08/2004", getNasc(p));

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

    FILE* txt = fopen("testeRIP.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeRIP.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    Pessoa* removida = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(removida);

    TEST_ASSERT_EQUAL_INT(-1, searchRegister(pessoasHash, "800.577.369-28", removida));

    free(removida);

    txt = fopen("testeRIP.txt", "r");
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

void testMud(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "mud 800.577.369-28 cep1 S 50 casa\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa("800.577.369-28", "Larissa", "Costa", 'F',"26/08/2004");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeMUD.txt", "w");
    FILE* svg = fopen("testeMUD.svg", "w");

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    Pessoa* resultado = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(resultado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "800.577.369-28", resultado));

    TEST_ASSERT_TRUE(pessoaTemMoradia(resultado));
    TEST_ASSERT_EQUAL_STRING("cep1", getCepMoradia(resultado));
    TEST_ASSERT_EQUAL_CHAR('S', getFaceMoradia(resultado));
    TEST_ASSERT_EQUAL_INT(50, getNumMoradia(resultado));
    TEST_ASSERT_EQUAL_STRING("casa", getCompMoradia(resultado));

    free(resultado);
    closeFile(pessoasHash);
}

void testDspj(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "dspj 800.577.369-28\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa("800.577.369-28", "Larissa", "Costa", 'F',"26/08/2004");

    setMoradia(p, "cep1", 'S', 45, "casa");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeDSPJ.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeDSPJ.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg);

    fclose(txt);
    fclose(svg);

    Pessoa* resultado = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(resultado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "800.577.369-28", resultado));
    TEST_ASSERT_FALSE(pessoaTemMoradia(resultado));

    free(resultado);

    txt = fopen("testeDSPJ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrouNome = 0;
    int encontrouEndereco = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrouNome = 1;
        }
        if (strstr(buffer, "cep1") != NULL) {
            encontrouEndereco = 1;
        }
    }

    TEST_ASSERT_TRUE(encontrouNome);
    TEST_ASSERT_TRUE(encontrouEndereco);

    fclose(txt);
    closeFile(pessoasHash);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testRq);
    RUN_TEST(testPq);
    RUN_TEST(testCenso);
    RUN_TEST(testH);
    RUN_TEST(testNasc);
    RUN_TEST(testRip);
    RUN_TEST(testMud);
    RUN_TEST(testDspj);

    return UNITY_END();
}