#include "hashfile.h"
#include <stdlib.h>
#include <string.h>

#define BUCKET_SIZE 2
#define KEY_SIZE 64
#define MAX_RECORD_SIZE 512

typedef struct {
    char keys[BUCKET_SIZE][KEY_SIZE];
    unsigned char records[BUCKET_SIZE][MAX_RECORD_SIZE];
    int count;
    int localDepth;
} Bucket;

typedef struct {
    int globalDepth;
    size_t recordSize;
} Header;

struct HashFile {
    FILE* hf;   // buckets
    FILE* hfc;  // cabeçalho + diretorio

    int globalDepth;
    long* directory;
    size_t recordSize;
};

int hashString(const char* key, int depth) {
    unsigned long h = 5381;

    for (int i = 0; key[i] != '\0'; i++) {
        h = ((h << 5) + h) + (unsigned char) key[i];
    }

    return h & ((1 << depth) - 1);
}

Bucket createBucket(int depth) {
    Bucket b;
    b.count = 0;
    b.localDepth = depth;
    return b;
}

long writeBucket(HashFile* h, Bucket* b) {
    fseek(h->hf, 0, SEEK_END);
    long pos = ftell(h->hf);
    fwrite(b, sizeof(Bucket), 1, h->hf);
    return pos;
}

void readBucket(HashFile* h, long pos, Bucket* b) {
    fseek(h->hf, pos, SEEK_SET);
    fread(b, sizeof(Bucket), 1, h->hf);
}

void saveHeader(HashFile* h) {
    rewind(h->hfc);

    Header head = { h->globalDepth, h->recordSize };
    fwrite(&head, sizeof(Header), 1, h->hfc);

    int size = 1 << h->globalDepth;
    fwrite(h->directory, sizeof(long), size, h->hfc);
}


HashFile* createFile(const char* name, size_t recordSize) {
    if (recordSize > MAX_RECORD_SIZE) {
        return NULL;
    }

    HashFile* h = malloc(sizeof(HashFile));
    if (!h) {
        return NULL;
    }
    
    h->recordSize = recordSize;

    char *hfName = malloc(strlen(name) + 4);
    char *hfcName = malloc(strlen(name) + 5);

    sprintf(hfName, "%s.hf", name);
    sprintf(hfcName, "%s.hfc", name);

    h->hf = fopen(hfName, "wb+");
    h->hfc = fopen(hfcName, "wb+");

    if (!h->hf || !h->hfc) {
        if (h->hf) fclose(h->hf);
        if (h->hfc) fclose(h->hfc);
        free(h);
        return NULL;
    }

    h->globalDepth = 1;
    int size = 2;
    h->directory = malloc(size * sizeof(long));

    if (!h->directory) {
        fclose(h->hf);
        fclose(h->hfc);
        free(h);
        return NULL;
    }

    Bucket b0 = createBucket(1);
    Bucket b1 = createBucket(1);

    h->directory[0] = writeBucket(h, &b0);
    h->directory[1] = writeBucket(h, &b1);

    saveHeader(h);
    free(hfName);
    free(hfcName);
    return h;
}


HashFile* openFile(const char* name) {
    HashFile* h = malloc(sizeof(HashFile));

    char *hfName = malloc(strlen(name) + 4);
    char *hfcName = malloc(strlen(name) + 5);
    sprintf(hfName, "%s.hf", name);
    sprintf(hfcName, "%s.hfc", name);

    h->hf = fopen(hfName, "rb+");
    h->hfc = fopen(hfcName, "rb+");

    Header head;
    fread(&head, sizeof(Header), 1, h->hfc);
    if (!h->hf || !h->hfc) {
        if (h->hf) fclose(h->hf);
        if (h->hfc) fclose(h->hfc);
        free(h);
        return NULL;
    }

    h->globalDepth = head.globalDepth;
    h->recordSize = head.recordSize;

    int size = 1 << h->globalDepth;
    h->directory = malloc(size * sizeof(long));

    if (!h->hf || !h->hfc) {
        if (h->hf) fclose(h->hf);
        if (h->hfc) fclose(h->hfc);
        free(hfName);
        free(hfcName);
        free(h);
        return NULL;
    }

    fread(h->directory, sizeof(long), size, h->hfc);

    free(hfName);
    free(hfcName);
    return h;
}

void doubleDirectory(HashFile* h) {
    int oldSize = 1 << h->globalDepth;
    int newSize = oldSize * 2;

    h->directory = realloc(h->directory, newSize * sizeof(long));

    for (int i = 0; i < oldSize; i++) {
        h->directory[i + oldSize] = h->directory[i];
    }

    h->globalDepth++;
}


int insertRegister(HashFile* h, const char* key, const void* record) {
    int index = hashString(key, h->globalDepth);
    
    Bucket b;
    long pos = h->directory[index];

    readBucket(h, pos, &b);

    for (int i = 0; i < b.count; i++) {
        if (strcmp(b.keys[i], key) == 0) {
            memcpy(b.records[i], record, h->recordSize);

            fseek(h->hf, pos, SEEK_SET);
            fwrite(&b, sizeof(Bucket), 1, h->hf);

            return 0;
        }
    }

    if (b.count < BUCKET_SIZE) {
        strncpy(b.keys[b.count], key, KEY_SIZE - 1);
        b.keys[b.count][KEY_SIZE - 1] = '\0';

        memcpy(b.records[b.count], record, h->recordSize);

        b.count++;
   

        fseek(h->hf, pos, SEEK_SET);
        fwrite(&b, sizeof(Bucket), 1, h->hf);

        return 0;
    }

    splitBucket(h, index);
    return insertRegister(h, key, record);
}


void splitBucket(HashFile* h, int index) {
    long oldPos = h->directory[index];

    Bucket old;
    readBucket(h, oldPos, &old);

    if (old.localDepth == h->globalDepth) {
        doubleDirectory(h);
    }

    Bucket newB = createBucket(old.localDepth + 1);
    old.localDepth++;

    long newPos = writeBucket(h, &newB);

    int size = 1 << h->globalDepth;

    for (int i = 0; i < size; i++) {
        if (h->directory[i] == oldPos &&
            ((i >> (old.localDepth - 1)) & 1)) {
            h->directory[i] = newPos;
        }
    }

    char tempKeys[BUCKET_SIZE][KEY_SIZE];
    unsigned char tempRecords[BUCKET_SIZE][MAX_RECORD_SIZE];
    int count = old.count;

    //copia os dados antigos
    for (int i = 0; i < count; i++) {
        strcpy(tempKeys[i], old.keys[i]);
        memcpy(tempRecords[i], old.records[i], h->recordSize);
    }

    // limpa o bucket antigo
    old.count = 0;
    fseek(h->hf, oldPos, SEEK_SET);
    fwrite(&old, sizeof(Bucket), 1, h->hf);

    // e reinsere 
    for (int i = 0; i < count; i++) {
        insertRegister(h, tempKeys[i], tempRecords[i]);
    }
}



int searchRegister(HashFile* h, const char* key, void* outRecord) {
    int index = hashString(key, h->globalDepth);

    Bucket b;
    readBucket(h, h->directory[index], &b);

    for (int i = 0; i < b.count; i++) {
        if (strcmp(b.keys[i], key) == 0) {
            memcpy(outRecord, b.records[i], h->recordSize);
            return 0;
        }
    }
    return -1;
}



int removeRegister(HashFile* h, const char* key) {
    int index = hashString(key, h->globalDepth);

    Bucket b;
    long pos = h->directory[index];

    readBucket(h, pos, &b);

    for (int i = 0; i < b.count; i++) {
        if (strcmp(b.keys[i], key) == 0) {
            strcpy(b.keys[i], b.keys[b.count - 1]);
            memcpy(b.records[i], b.records[b.count - 1], h->recordSize);
            b.count--;

            fseek(h->hf, pos, SEEK_SET);
            fwrite(&b, sizeof(Bucket), 1, h->hf);

            return 0;
        }
    }

    return -1;
}



void generateHFD(HashFile* h, const char* filename) {
    FILE* out = fopen(filename, "w");

    if (!out) {
        printf("Erro ao criar .hfd\n");
        return;
    }

    fprintf(out, "===== HASHFILE DUMP =====\n\n");

    fprintf(out, "Global Depth: %d\n\n", h->globalDepth);

    int size = 1 << h->globalDepth;

    // Diretório
    fprintf(out, "DIRECTORY:\n");
        for (int i = 0; i < size; i++) {
    fprintf(out, "[%d] -> Bucket @ %ld\n", i, h->directory[i]);
    }

    // Mostra os buckets
    fprintf(out, "\nBUCKETS:\n");
    
    //Pra nao ter bucket repetido
    for (int i = 0; i < size; i++) {
        long pos = h->directory[i];

        int alreadyPrinted = 0;
        for (int j = 0; j < i; j++) {
            if (h->directory[j] == pos) {
                alreadyPrinted = 1;
                break;
            }
        }

        if (alreadyPrinted) continue;

        Bucket b;
        readBucket(h, pos, &b);

        fprintf(out, "\nBucket @ %ld\n", pos);
        fprintf(out, "Local Depth: %d\n", b.localDepth);
        fprintf(out, "Count: %d\n", b.count);

        for (int k = 0; k < b.count; k++) {
            fprintf(out, "  key: %s\n", b.keys[k]);
        }
    }

    fprintf(out, "\n=========================\n");

    fclose(out);
}


void closeFile(HashFile* h) {
    saveHeader(h);

    generateHFD(h, "saida.hfd");

    fclose(h->hf);
    fclose(h->hfc);

    free(h->directory);
    free(h);
}


void scanRegisters(HashFile* h, HashScanCallback callback, void* extra) {
    int size = 1 << h->globalDepth;

    for (int i = 0; i < size; i++) {
        long pos = h->directory[i];

        int repetido = 0;
        for (int j = 0; j < i; j++) {
            if (h->directory[j] == pos) {
                repetido = 1;
                break;
            }
        }

        if (repetido) continue;

        Bucket b;
        readBucket(h, pos, &b);

        for (int k = 0; k < b.count; k++) {
            callback(b.keys[k], b.records[k], extra);
        }
    }
}