#include "tools.h"

int 
min(int a, int b) {
    return (a > b) ? b : a;
}

int 
max(int a, int b) {
    return (a <= b) ? b : a;
}

float 
minf(float a, float b) {
    return (a > b) ? b : a;
}

float 
maxf(float a, float b) {
    return (a <= b) ? b : a;
}

float 
__getDistance(char *s1, int lens1, char *s2, int lens2) {
    int differences = 0;
    for (int i = 0; i < minf(lens1, lens2); i++)
        differences += (s1[i] != s2[i]);
    return -0.75 * (log(1.0f - (4.0f / 3.0f) * (differences / (maxf(lens1, lens2) * 1.0f))));
}

float 
getDistance(char *s1, char *s2) {
    return __getDistance(s1, strlen(s1), s2, strlen(s2));    
}

void 
printDistanceMatrix(int sz, float **distanceMatrix) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            printf("| %5.5f |", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}

void 
fillDistanceMatrix(int nSequences, char **sequences, float **distanceMatrix) {
    for (int i = 0; i < nSequences; i++) {
        for (int j = 0; j < nSequences; j++) {
            distanceMatrix[i][j] = getDistance(sequences[i], sequences[j]);
        }
    }
}

void 
fillDistanceMatrixFromDistanceMatrix(int nSequences, float **inDistanceMatrix, float **outDistanceMatrix, int ignoreidx) {
    int ti, tj;
    for (int i = 0; i < nSequences; i++) {
        if (i == ignoreidx)
            continue;
        for (int j = 0; j < nSequences; j++) {
            if (j == ignoreidx)
                continue;
            ti = (i < ignoreidx) ? i : i - 1;
            tj = (j < ignoreidx) ? j : j - 1;
            outDistanceMatrix[ti][tj] = inDistanceMatrix[i][j];
        }
    }
}

void 
freeMatrix(float **matrix, int size) {
    free(matrix[0]);
    free(matrix);
}

float**
allocateMatrix(int size) {
    float** matrix = (float**)malloc(size * sizeof(float*));
    float* data = (float*)malloc(size * size * sizeof(float));
    for (int i = 0; i < size; i++) {
        matrix[i] = data + (i * size);
    }
    return matrix;
}
