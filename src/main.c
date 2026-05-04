#include "../include/geo.h"
#include "../include/pm.h"
#include "../include/qry.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include "../include/pessoa.h"
#include "../include/svg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void montarPath(char* destino, const char* dir, const char* arq) {
    if (dir == NULL || strlen(dir) == 0) {
        sprintf(destino, "%s", arq);
    } else {
        sprintf(destino, "%s/%s", dir, arq);
    }
}


int main(int argc, char* argv[]) {
    char bed[256] = "";
    char bsd[256] = "";
    char arqGeo[256] = "";
    char arqPm[256] = "";
    char arqQry[256] = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            strcpy(bed, argv[++i]);
        } 

        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strcpy(arqGeo, argv[++i]);
        } 

        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strcpy(bsd, argv[++i]);
        } 

        else if (strcmp(argv[i], "-pm") == 0 && i + 1 < argc) {
            strcpy(arqPm, argv[++i]);
        } 

        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            strcpy(arqQry, argv[++i]);
        }
    }

    if (strlen(arqGeo) == 0 || strlen(bsd) == 0) {
        printf("Uso: ./prog -e entrada -f cidade.geo -pm pessoas.pm -q consulta.qry -o saida\n");
        return 1;
    }

    #ifdef _WIN32
        char comando[300];
        sprintf(comando, "mkdir \"%s\" 2>nul", bsd);
        system(comando);
    #else
        char comando[300];
        sprintf(comando, "mkdir \"%s\" 2>nul", bsd);
        system(comando);
    #endif
    
    
    char geoPath[512];
    char pmPath[512];
    char qryPath[512];
    char svgPath[512];
    char txtPath[512];

    montarPath(geoPath, bed, arqGeo);

    if (strlen(arqPm) > 0) {
        montarPath(pmPath, bed, arqPm);
    }

    if (strlen(arqQry) > 0) {
        montarPath(qryPath, bed, arqQry);
    }

    sprintf(svgPath, "%s/saida.svg", bsd);
    sprintf(txtPath, "%s/saida.txt", bsd);

    HashFile* quadras = createFile("quadras", getQuadraSize());
    HashFile* pessoas = createFile("pessoas", getPessoaSize());

    if (!quadras || !pessoas) {
        printf("Erro ao criar hashfiles\n");

        if (quadras) closeFile(quadras);
        if (pessoas) closeFile(pessoas);

        return 1;
    }

    FILE* svg = fopen(svgPath, "w");
    FILE* txt = fopen(txtPath, "w");

    if (!svg || !txt) {
        printf("Erro ao criar arquivos de saída\n");

        if (svg) fclose(svg);
        if (txt) fclose(txt);

        closeFile(quadras);
        closeFile(pessoas);

        return 1;
    }

    startSVG(svg);

    processGeo(geoPath, quadras, svg);

    if (strlen(arqPm) > 0) {
        processPM(pmPath, pessoas);
    }

    if (strlen(arqQry) > 0) {
        processQry(qryPath, pessoas, quadras, txt, svg);
    }

    endSVG(svg);

    fclose(svg);
    fclose(txt);

    char quadrasHfdPath[512];
    char pessoasHfdPath[512];

    sprintf(quadrasHfdPath, "%s/quadras.hfd", bsd);
    sprintf(pessoasHfdPath, "%s/pessoas.hfd", bsd);

    generateHFD(quadras, quadrasHfdPath);
    generateHFD(pessoas, pessoasHfdPath);

    closeFile(quadras);
    closeFile(pessoas);

    return 0;
}