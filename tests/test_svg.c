#include <stdio.h>
#include "../include/svg.h"

int main() {
    FILE* svg = fopen("teste.svg", "w");

    startSVG(svg);

    drawQuadra(svg, "CEP1", 50, 20, 200, 150, "gray", "black", 5);
    drawQuadra(svg, "CEP2", 400, 20, 200, 150, "pink", "black", 5);

    endSVG(svg);

    fclose(svg);
    return 0;
}