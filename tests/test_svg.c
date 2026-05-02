#include "../Unity/unity.h"
#include "../include/svg.h"
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste.svg");
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

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCriacao);
    RUN_TEST(testDesenho);

    return UNITY_END();
}