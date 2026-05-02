#ifndef PM_H
#define PM_H

#include "hashfile.h"

/** Processa arquivo .pm contendo pessoas e moradores
 * @param pmPath Caminho do arquivo .pm
 * @param pessoasHash Hashfile onde as pessoas serão armazenadas
 */
void processPM(const char* pmPath, HashFile* pessoasHash);

#endif