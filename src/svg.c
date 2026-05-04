#include "../include/svg.h"
#include <stdio.h>


void startSVG(FILE* svg) {
    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
}


void endSVG(FILE* svg) {
    fprintf(svg, "</svg>\n");
}


void drawQuadra(FILE* svg, const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, double sw) {

    //Quadra
    fprintf(svg, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" " "fill=\"%s\" stroke=\"%s\" stroke-width=\"%lf\" />\n", x, y, w, h, cfill, cstrk, sw);

    //Ponto de âncora
    fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"10\" fill=\"red\" stroke=\"%s\" stroke-width=\"%lf\" />\n", x, y, cstrk, sw);

    //CEP
    fprintf(svg, "<text x=\"%lf\" y=\"%lf\" font-size=\"10\" fill=\"black\">%s</text>\n", x + w/4, y + h/2, cep);
}


// dos comandos
void drawXVermelho(FILE* svg, double x, double y) {
    double t = 10;

    fprintf(svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x - t, y - t, x + t, y + t);

    fprintf(svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x - t, y + t, x + t, y - t);
}
