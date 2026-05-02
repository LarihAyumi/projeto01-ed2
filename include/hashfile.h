#ifndef HASHFILE_H
#define HASHFILE_H

//Adaptação de hashfile extensível em memória para versão com persistência em disco (.hf e .hfc)

#include <stdio.h>

//Estrutura arquivo hash
typedef struct HashFile HashFile;

/** Cria um arquivo hash
 * @param filename Nome do arquivo
 * @return Ponteiro para HashFile ou NULL quando erro
 */
HashFile* createFile(const char* filename);


/** Abre um arquivo hash existente
 * @param filename Nome do arquivo
 * @return Ponteiro para HashFile/NULL
 */
HashFile* openFile(const char* filename);

/** Insere um registro
 * @param file Estrutura do arquivo
 * @param key Chave
 * @param value Valor
 * @return 0 = sucesso, -1= erro
 */
int insertRegister(HashFile* file, int key, int value);


/** Busca um registro
 * @param file Estrutura do arquivo
 * @param key Chave
 * @param value Ponteiro para retorno
 * @return 0 = encontrado, -1 = não encontrado
 */
int searchRegister(HashFile* file, int key, int* value);

/** Remove um registro
 * @param file Estrutura hash
 * @param key Chave
 * @return 0 = sucesso, -1 = erro
 */
int removeRegister(HashFile* file, int key);

/** Gera arquivo HFD com a representação do hashfile
 * @param h estrutura do hashfile
 * @param filename nome do arquivo que será gerado hfd
 */
void generateHFD(HashFile* h, const char* filename);

/** Fecha o hashfile, salva o cabeçalho e libera a memória
 * @param file estrutura do hashfile
 */
void closeFile(HashFile* file);

#endif 