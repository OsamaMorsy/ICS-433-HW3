#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REF_STRING_LENGTH 20

void generateReferenceString(int refString[], int length) {
    for (int i = 0; i < length; i++) {
        refString[i] = rand() % 10;
    }
}

void printReferenceString(int refString[], int length) {
    printf("Reference String: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", refString[i]);
    }
    printf("\n");
}

int isInMemory(int page, int memory[], int frames) {
    for (int i = 0; i < frames; i++) {
        if (memory[i] == page) return i;
    }
    return -1;
}

int findFIFOReplacement(int* fifoQueue, int frames) {
    int victim = fifoQueue[0];
    for (int i = 0; i < frames - 1; i++) {
        fifoQueue[i] = fifoQueue[i + 1];
    }
    return victim;
}

int findLRUReplacement(int* lastUsed, int frames) {
    int min = lastUsed[0], index = 0;
    for (int i = 1; i < frames; i++) {
        if (lastUsed[i] < min) {
            min = lastUsed[i];
            index = i;
        }
    }
    return index;
}

int findOPTReplacement(int refString[], int currentIndex, int memory[], int frames, int length) {
    int max = -1, index = -1, found;
    for (int i = 0; i < frames; i++) {
        found = 0;
        for (int j = currentIndex + 1; j < length; j++) {
            if (memory[i] == refString[j]) {
                found = 1;
                if (j > max) {
                    max = j;
                    index = i;
                }
                break;
            }
        }
        if (!found) return i;
    }
    return index;
}

void simulate(int refString[], int frames, int length, int algo) {
    int memory[frames], lastUsed[frames], fifoQueue[frames], faults = 0, index = 0;
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        lastUsed[i] = -1;
        fifoQueue[i] = -1;
    }

    for (int i = 0; i < length; i++) {
        int page = refString[i];
        int loc = isInMemory(page, memory, frames);

        if (loc == -1) { // Page fault
            if (index < frames) {
                memory[index] = page;
                lastUsed[index] = i;
                fifoQueue[index] = page;
                index++;
            } else {
                int replacementIndex;
                switch (algo) {
                    case 0: // FIFO
                        replacementIndex = findFIFOReplacement(fifoQueue, frames);
                        break;
                    case 1: // LRU
                        replacementIndex = findLRUReplacement(lastUsed, frames);
                        break;
                    case 2: // OPT
                        replacementIndex = findOPTReplacement(refString, i, memory, frames, length);
                        break;
                    default:
                        replacementIndex = 0;
                }
                memory[replacementIndex] = page;
                lastUsed[replacementIndex] = i;
                fifoQueue[replacementIndex] = page;
            }
            faults++;
        } else {
            lastUsed[loc] = i;
        }
    }
    printf("Algorithm: %s, Page faults: %d\n", algo == 0 ? "FIFO" : (algo == 1 ? "LRU" : "OPT"), faults);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of frames>\n", argv[0]);
        return 1;
    }

    int frames = atoi(argv[1]);
    int refString[REF_STRING_LENGTH];

    srand(time(NULL));

    generateReferenceString(refString, REF_STRING_LENGTH);
    printReferenceString(refString, REF_STRING_LENGTH);

    simulate(refString, frames, REF_STRING_LENGTH, 0); // FIFO
    simulate(refString, frames, REF_STRING_LENGTH, 1); // LRU
    simulate(refString, frames, REF_STRING_LENGTH, 2); // OPT

    return 0;
}
