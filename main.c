#include <float.h>
#include <stdio.h>
#include "tools.h"
#include "node.h"

typedef struct tuple {
    Node **node;
    int sz;
} tuple;

tuple
upgma(int n, float **distanceMatrix)
{
    int szCluster = n;
    Node** clusters = (Node**)malloc(szCluster * sizeof(Node*));
    if (!clusters) exit(-1);
    
    for (int i = 0; i < szCluster; i++) {
        clusters[i] = create_value_node(i);
    }

    while (n > 2) {
        int idx1, idx2;
        float curmin = FLT_MAX;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (curmin > distanceMatrix[i][j]) {
                    curmin = distanceMatrix[i][j];
                    idx1 = max(i, j);
                    idx2 = min(i, j);
                }
            }
        }

        float **distanceMatrix2 = allocateMatrix(n - 1);
        if (!distanceMatrix2) exit(-1);

        fillDistanceMatrixFromDistanceMatrix(n, distanceMatrix, distanceMatrix2, idx1);

        for (int i = 0; i < n; i++) {
            if (i == idx1 || i == idx2)
                continue;
            int ti = (i < idx1) ? i : (i < idx2 ? i : i - 1);
            int nClusters = clusters[idx1]->size + clusters[idx2]->size;

            distanceMatrix2[max(ti, idx2)][min(ti, idx2)] = ((distanceMatrix[max(i, idx2)][min(i, idx2)] * clusters[idx2]->size) + (distanceMatrix[max(i, idx1)][min(i, idx1)] * clusters[idx1]->size)) / nClusters;
            distanceMatrix2[min(ti, idx2)][max(ti, idx2)] = distanceMatrix2[max(ti, idx2)][min(ti, idx2)];
        }

        clusters[idx2] = create_cluster_node(clusters[idx2], clusters[idx1]);
        clusters[idx2]->length[0] = distanceMatrix[idx1][idx2] / 2.0f;
        clusters[idx2]->length[1] = distanceMatrix[idx1][idx2] / 2.0f;
        resize_clusters(clusters, &szCluster, idx1);

        freeMatrix(distanceMatrix, n);
        distanceMatrix = distanceMatrix2;

        printDistanceMatrix(n - 1, distanceMatrix2);
        printf("\n");

        n--;
    }

    freeMatrix(distanceMatrix, n);

    return (tuple){clusters, szCluster};
}

tuple
neighborJoining(int n, float **distanceMatrix)
{
    int szCluster = n;
    Node** clusters = (Node**)malloc(szCluster * sizeof(Node*));
    if (!clusters) exit(-1);

    for (int i = 0; i < szCluster; i++) {
        clusters[i] = create_value_node(i);
    }

    while (n > 2) {
        int mini = -1, minj = -1;
        float mindik = 0, mindjk = 0, curmin = FLT_MAX;
        float **qmatrix = allocateMatrix(5);
        if (!qmatrix) exit(-1);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    qmatrix[i][j] = 0.0f;
                    continue;
                }

                float dik = 0, djk = 0;
                for (int k = 0; k < n; k++) {
                    dik += distanceMatrix[i][k];
                    djk += distanceMatrix[j][k];
                }

                float curval = (n - 2) * distanceMatrix[i][j] - dik - djk;
                qmatrix[i][j] = curval;

                if (curmin > curval) {
                    curmin = curval;
                    mindik = dik;
                    mindjk = djk;
                    mini = i;
                    minj = j;
                }
            }
        }

        if (mini == -1 || minj == -1) exit(-1);

        clusters[mini] = create_cluster_node(clusters[mini], clusters[minj]);
        clusters[mini]->length[0] = (0.5 * distanceMatrix[mini][minj]) + ((1.0f / (2.0f * (n - 2.0f))) * (mindjk - mindik));
        clusters[mini]->length[1] = distanceMatrix[mini][minj] - clusters[mini]->length[0];
        resize_clusters(clusters, &szCluster, minj);

        float **distanceMatrix2 = allocateMatrix(n - 1);
        if (!distanceMatrix2) exit(-1);

        fillDistanceMatrixFromDistanceMatrix(n, distanceMatrix, distanceMatrix2, mini);

        for (int i = 0; i < n; i++) {
            if (i == mini || i == minj) continue;
            int ti = (i < mini) ? i : (i < minj ? i : i - 1);

            distanceMatrix2[mini][ti] = 0.5 * (distanceMatrix[mini][i] + distanceMatrix[minj][i] - distanceMatrix[mini][minj]);
            distanceMatrix2[ti][mini] = distanceMatrix2[mini][ti];
        }

        printf("Qmatrix:\n");
        printDistanceMatrix(n, qmatrix);
        freeMatrix(qmatrix, n);
        printf("New Distance Matrix:\n");
        printDistanceMatrix(n - 1, distanceMatrix2);

        freeMatrix(distanceMatrix, n);
        distanceMatrix = distanceMatrix2;
        n--;
    }

    freeMatrix(distanceMatrix, n);

    return (tuple){clusters, szCluster};
}

int 
main() {
    int nSequences = 14;
    int maxSequence = 20;

    char **sequences = (char**)malloc(nSequences * sizeof(char*));
    for (int i = 0; i < nSequences; i++) {
        sequences[i] = (char*)malloc((maxSequence + 1) * sizeof(char));
    }

    /*
    //int nSequences = 14;
    strcpy(sequences[0], "ATTGCCATT");
    strcpy(sequences[1], "ATGGCCATT");
    strcpy(sequences[2], "ATCCAATTTT");
    strcpy(sequences[3], "ATCTTCTT");
    strcpy(sequences[4], "ACTGACC");
    */

    // APELLIDOS
    strcpy(sequences[0], "concha");
    strcpy(sequences[1], "sifuentes");
    strcpy(sequences[2], "flores");
    strcpy(sequences[3], "melendez");
    strcpy(sequences[4], "pino");
    strcpy(sequences[5], "zavala");
    strcpy(sequences[6], "quispe");
    strcpy(sequences[7], "neira");
    strcpy(sequences[8], "salas");
    strcpy(sequences[9], "ticona");
    strcpy(sequences[10], "tupac");
    strcpy(sequences[11], "valdivia");
    strcpy(sequences[12], "villanueva");
    strcpy(sequences[13], "borda");

    int size = nSequences;
    float **distanceMatrix = allocateMatrix(nSequences);
    //fillDistanceMatrix(nSequences, sequences, distanceMatrix);
    
    // UPGMA
    float values[5][5] = {
        {0, 17, 21, 31, 23},
        {17, 0, 30, 34, 21},
        {21, 30, 0, 28, 39},
        {31, 34, 28, 0, 43},
        {23, 21, 39, 43, 0}
    };

    // NJ
    /*
    float values[5][5] = {
        {0, 5, 9, 9, 8},
        {5, 0, 10, 10, 9},
        {9, 10, 0, 8, 7},
        {9, 10, 8, 0, 3},
        {8, 9, 7, 3, 0}
    };
    */

    for (int i = 0; i < nSequences; i++) {
        for (int j = 0; j < nSequences; j++) {
            distanceMatrix[i][j] = values[i][j];
        }
    }

    // tuple res = upgma(nSequences, distanceMatrix);
    tuple res = neighborJoining(nSequences, distanceMatrix);
    
    printCluster3(res.sz, res.node, nSequences, sequences);

    return 0;
}
