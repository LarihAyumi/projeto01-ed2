#include "hashfile.h"
#include <stdlib.h>
#include <string.h>

#define BUCKET_SIZE 2

typedef struct {
    int keys[BUCKET_SIZE];
    int values[BUCKET_SIZE];
    int count;
    int localDepth;
} Bucket;

typedef struct {
    int globalDepth;
} Header;

struct HashFile {
    FILE* hf;   // buckets
    FILE* hfc;  // cabeçalho + diretorio

    int globalDepth;
    long* directory;
};

int hash(int key, int depth) {
    return key & ((1 << depth) - 1);
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

    Header head = { h->globalDepth };
    fwrite(&head, sizeof(Header), 1, h->hfc);

    int size = 1 << h->globalDepth;
    fwrite(h->directory, sizeof(long), size, h->hfc);
}

HashFile* createFile(const char* name) {
    HashFile* h = malloc(sizeof(HashFile));

    char hfName[50], hfcName[50];
    sprintf(hfName, "%s.hf", name);
    sprintf(hfcName, "%s.hfc", name);

    h->hf = fopen(hfName, "wb+");
    h->hfc = fopen(hfcName, "wb+");

    h->globalDepth = 1;
    int size = 2;

    h->directory = malloc(size * sizeof(long));

    Bucket b0 = createBucket(1);
    Bucket b1 = createBucket(1);

    h->directory[0] = writeBucket(h, &b0);
    h->directory[1] = writeBucket(h, &b1);

    saveHeader(h);
    return h;
}

HashFile* openFile(const char* name) {
    HashFile* h = malloc(sizeof(HashFile));

    char hfName[50], hfcName[50];
    sprintf(hfName, "%s.hf", name);
    sprintf(hfcName, "%s.hfc", name);

    h->hf = fopen(hfName, "rb+");
    h->hfc = fopen(hfcName, "rb+");

    Header head;
    fread(&head, sizeof(Header), 1, h->hfc);

    h->globalDepth = head.globalDepth;

    int size = 1 << h->globalDepth;
    h->directory = malloc(size * sizeof(long));

    fread(h->directory, sizeof(long), size, h->hfc);

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

    int tempKeys[BUCKET_SIZE];
    int tempValues[BUCKET_SIZE];
    int count = old.count;

    for (int i = 0; i < count; i++) {
        tempKeys[i] = old.keys[i];
        tempValues[i] = old.values[i];
    }

    old.count = 0;
    fseek(h->hf, oldPos, SEEK_SET);
    fwrite(&old, sizeof(Bucket), 1, h->hf);

    for (int i = 0; i < count; i++) {
        insertRegister(h, tempKeys[i], tempValues[i]);
    }
}

int insertRegister(HashFile* h, int key, int value) {
    int index = hash(key, h->globalDepth);

    Bucket b;
    long pos = h->directory[index];

    readBucket(h, pos, &b);

    if (b.count < BUCKET_SIZE) {
        b.keys[b.count] = key;
        b.values[b.count] = value;
        b.count++;

        fseek(h->hf, pos, SEEK_SET);
        fwrite(&b, sizeof(Bucket), 1, h->hf);

        return 0;
    }

    splitBucket(h, index);
    return insertRegister(h, key, value);
}

int searchRegister(HashFile* h, int key, int* value) {
    int index = hash(key, h->globalDepth);

    Bucket b;
    readBucket(h, h->directory[index], &b);

    for (int i = 0; i < b.count; i++) {
        if (b.keys[i] == key) {
            *value = b.values[i];
            return 0;
        }
    }

    return -1;
}

int removeRegister(HashFile* h, int key) {
    int index = hash(key, h->globalDepth);

    Bucket b;
    long pos = h->directory[index];

    readBucket(h, pos, &b);

    for (int i = 0; i < b.count; i++) {
        if (b.keys[i] == key) {
            b.keys[i] = b.keys[b.count - 1];
            b.values[i] = b.values[b.count - 1];
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

    //Diretório
    fprintf(out, "DIRECTORY:\n");
    for (int i = 0; i < size; i++) {
        fprintf(out, "[%d] -> Bucket @ %ld\n", i, h->directory[i]);
    }

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
            fprintf(out, "  (%d -> %d)\n", b.keys[k], b.values[k]);
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