#ifndef GEO_H
#define GEO_H

#include "hashfile.h"

/** Módulo responsável pelo processamento do arquivo .geo.
 *  O arquivo .geo descreve a cidade (quadras) e define parâmetros visuais.
 *  As quadras podem ser armazenadas em hashfile e também desenhadas em SVG.
 */

/** Processa o arquivo .geo e gera o SVG da cidade
 * @param geoPath Caminho do arquivo .geo
 * @param svgPath Caminho do arquivo .svg de saída
 * @param quadrasHash Hashfile para armazenar quadras
 */
void processGeo(const char* geoPath, const char* svgPath, HashFile* quadrasHash);



#endif