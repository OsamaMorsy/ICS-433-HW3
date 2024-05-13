#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REF_STRING_LENGTH 20 // Length of the page reference string

void generateReferenceString(int refString[], int length) {
    for (int i = 0; i < length; i++) {
        refString[i] = rand() % 10; // Generate numbers from 0 to 9
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

int findOPTReplacement(int refString[], int currentIndex, int memory[], int frames, int length) {
    int max = currentIndex;
    int index = -1;
    for (int i = 0; i < frames; i++) {
        int j;
        for (j = currentIndex + 1; j < length; j++) {
            if (refString[j] == memory[i]) {
                if (j > max) {
                    max = j;
                    index = i;
                }
                break;
            }
        }
        if (j == length) return i; // Not found in future, so replace this
    }
    return index; // Page that will not be used for the longest time
}

int findLRUReplacement(int used[], int frames) {
    int min = used[0];
    int index = 0;
    for (int i = 1; i < frames; i++) {
        if (used[i] < min) {
            min = used[i];
            index = i;
        }
    }
    return index;
}

void simulatePageReplacement(int refString[], int frames, int length) {
    int memory[frames], used[frames], faults = 0, index = 0;
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        used[i] = 0;
    }

    for (int i = 0; i < length; i++) {
        int page = refString[i];
        int loc = isInMemory(page, memory, frames);

        if (loc == -1) { // Page fault
            if (index < frames) {
                memory[index] = page;
                used[index] = i; // Update recently used
                index++;
            } else {
                int replacementIndex = findOPTReplacement(refString, i, memory, frames, length);
                memory[replacementIndex] = page;
                used[replacementIndex] = i; // Update recently used
            }
            faults++;
        } else {
            used[loc] = i; // Update recently used
        }
    }

    printf("Page faults: %d\n", faults);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of frames>\n", argv[0]);
        return 1;
    }

    int frames = atoi(argv[1]);
    int refString[REF_STRING_LENGTH];

    srand(time(NULL)); // Seed the random number generator

    generateReferenceString(refString, REF_STRING_LENGTH);
    printReferenceString(refString, REF_STRING_LENGTH);

    printf("FIFO Algorithm:\n");
    simulatePageReplacement(refString, frames, REF_STRING_LENGTH);

    printf("LRU Algorithm:\n");
    simulatePageReplacement(refString, frames, REF_STRING_LENGTH);

    printf("OPT Algorithm:\n");
    simulatePageReplacement(refString, frames, REF_STRING_LENGTH);

    return 0;
}
