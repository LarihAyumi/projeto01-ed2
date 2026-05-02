#ifndef QUADRA_H
#define QUADRA_H
#include <stdio.h>

/** Módulo responsável pela criação e armazenamento de quadras.
 *  As quadras são representadas internamente e podem ser salvas em arquivo binário.
 */

//Struct da quadra
typedef struct Quadra Quadra;

/** Cria uma quadra
 * @param cep Identificador da quadra
 * @param x Coordenada x da âncora
 * @param y Coordenada y da âncora
 * @param w Largura da quadra
 * @param h Altura da quadra
 * @return Ponteiro para a quadra criada
 */
Quadra* createQuadra(const char* cep, double x, double y, double w, double h);

/** Salva a quadra em arquivo binário
 * @param q Estrutura quadra
 * @param file Arquivo para salvar a quadra
 * @param offset Retorna a posição da quadra no arquivo
 */
void saveQuadra(Quadra* q, FILE* file, long* offset);

#endif