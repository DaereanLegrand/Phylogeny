#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int min(int, int);
int max(int, int);
float minf(float, float);
float maxf(float, float);
float __getDistance(char*, int, char*, int);
float getDistance(char*, char*);
void printDistanceMatrix(int, float**);
void fillDistanceMatrix(int, char**, float**);
void fillDistanceMatrixFromDistanceMatrix(int, float**, float**, int);
void freeMatrix(float**, int);
float** allocateMatrix(int);

#endif
