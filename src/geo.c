#include "../include/geo.h"
#include "../include/svg.h"
#include "../include/quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void processGeo(const char* geoPath, const char* svgPath, HashFile* quadrasHash) {
    FILE* geo = fopen(geoPath, "r");
    FILE* svg = fopen(svgPath, "w");
    FILE* quadrasFile = fopen("quadras.dat", "ab+");

    if (!geo || !svg || !quadrasFile) {
        printf("Erro ao abrir arquivos\n");
        return;
    }

    char comando[10];

    //estado do cq
    double sw = 1.0;
    char cfill[20] = "white";
    char cstrk[20] = "black";

    startSVG(svg);

    while (fscanf(geo, "%s", comando) != EOF) {
        if (strcmp(comando, "cq") == 0) {
            fscanf(geo, "%lf %s %s", &sw, cfill, cstrk);
        }

        else if (strcmp(comando, "q") == 0) {
            char cep[20];
            double x, y, w, h;

            fscanf(geo, "%s %lf %lf %lf %lf",
                   cep, &x, &y, &w, &h);

            //desenha no SVG
            drawQuadra(svg, cep, x, y, w, h, cfill, cstrk, sw);

            if (quadrasHash != NULL) {
                Quadra* q = createQuadra(cep, x, y, w, h);
                insertRegister(quadrasHash, cep, q);
                free(q);
            }
        }
    }
    
    endSVG(svg);

    fclose(quadrasFile);
    fclose(geo);
    fclose(svg);
}