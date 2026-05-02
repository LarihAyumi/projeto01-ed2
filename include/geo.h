#ifndef GEO_H
#define GEO_H

#include "hashfile.h"

/** Processa o arquivo .geo e gera o SVG da cidade
 * @param geoPath Caminho do arquivo .geo
 * @param svgPath Caminho do arquivo .svg de saída
 * @param quadrasHash Hashfile para armazenar quadras
 */
void processGeo(const char* geoPath, const char* svgPath, HashFile* quadrasHash);



#endif