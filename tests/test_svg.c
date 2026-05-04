#include "../Unity/unity.h"
#include "../include/svg.h"
#include <string.h>
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste.svg");
    remove("testDrawXVermelho.svg");
    remove("testDrawTextoSVG.svg");
}


void testCriacao(void) {
    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    endSVG(svg);

    fclose(svg);

    svg = fopen("teste.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);
}


void testDesenho(void) {
    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "CEP1", 50, 20, 200, 150, "gray", "black", 5);
    drawQuadra(svg, "CEP2", 400, 20, 200, 150, "pink", "black", 5);

    endSVG(svg);
    fclose(svg);

    svg = fopen("teste.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);
}

void testDrawXVermelho(void) {
    FILE* svg = fopen("testDrawXVermelho.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "cep1", 100, 100, 100, 100, "pink", "black", 1);
    drawXVermelho(svg, 100, 100);

    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawXVermelho.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "<line") != NULL) {
            encontrou = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrou);
}

void testDrawTextoSVG(void) {
    FILE* svg = fopen("testDrawTextoSVG.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    drawTextoSVG(svg, 100, 100, "oi oi oi :)");
    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawTextoSVG.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "oi oi oi :)") != NULL) {
            encontrou = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrou);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCriacao);
    RUN_TEST(testDesenho);
    RUN_TEST(testDrawXVermelho);
    RUN_TEST(testDrawTextoSVG);

    return UNITY_END();
}