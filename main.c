#include <float.h>
#include <stdio.h>
#include "tools.h"
#include "node.h"

void
upgma(int n, float **distanceMatrix)
{
    int szCluster = n;
    Node** clusters = (Node**)malloc(szCluster * sizeof(Node*));
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
        fillDistanceMatrixFromDistanceMatrix(n, distanceMatrix, distanceMatrix2, idx1);

        int ti;
        for (int i = 0; i < n; i++) {
            if (i == idx1 || i == idx2)
                continue;
            ti = (i < idx1) ? i : (i < idx2 ? i : i - 1);
            int nClusters = clusters[idx1]->size + clusters[idx2]->size;

            distanceMatrix2[max(ti, idx2)][min(ti, idx2)] = ((distanceMatrix[max(i, idx2)][min(i, idx2)] * clusters[idx2]->size) + (distanceMatrix[max(i, idx1)][min(i, idx1)] * clusters[idx1]->size)) / nClusters;
        }

        clusters[idx2] = create_cluster_node(clusters[idx2], clusters[idx1]);
        resize_clusters(clusters, &szCluster, idx1);

        printDistanceMatrix(n - 1, distanceMatrix2);

        n--;
        distanceMatrix = distanceMatrix2;
    }

    printCluster(szCluster, clusters);
}

void
neighborJoining(int n, float **distanceMatrix)
{
    int szCluster = n;
    Node** clusters = (Node**)malloc(szCluster * sizeof(Node*));
    for (int i = 0; i < szCluster; i++) {
        clusters[i] = create_value_node(i);
    }

    while (n > 2) {
        int mini = -1;
        int minj = -1;
        float mindik = 0;
        float mindjk = 0;
        float curmin = FLT_MAX;
        float **qmatrix = allocateMatrix(5);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    continue;

                float dik = 0;
                float djk = 0;

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

        if (mini == -1 || minj == -1) {
            exit(-1);
        }
        
        clusters[mini] = create_cluster_node(clusters[mini], clusters[minj]);
        resize_clusters(clusters, &szCluster, minj);

        clusters[mini]->length[0] = (0.5 * distanceMatrix[mini][minj]) + ((1.0f / (2.0f * (n - 2.0f))) * (mindjk - mindik));
        clusters[mini]->length[1] = distanceMatrix[mini][minj] - clusters[mini]->length[0];

        float **distanceMatrix2 = allocateMatrix(n - 1);
        fillDistanceMatrixFromDistanceMatrix(n, distanceMatrix, distanceMatrix2, mini);

        int ti;
        for (int i = 0; i < n; i++) {
            if (i == mini || i == minj)
                continue;
            ti = (i < mini) ? i : (i < minj ? i : i - 1);
            
            int minidx = min(ti, minj);
            int maxidx = max(ti, minj);
            distanceMatrix2[maxidx][minidx] = 0.5 * (distanceMatrix[mini][i] + distanceMatrix[minj][i] - distanceMatrix[mini][minj]);
        }
        
        printDistanceMatrix(n, qmatrix);
        printf("mini: %d, minj: %d, mindik: %.2f, mindjk: %.2f, curmin: %.2f\n", mini, minj, mindik, mindjk, curmin);
        printDistanceMatrix(n - 1, distanceMatrix2);
        printf("\n");

        n--;
        distanceMatrix = distanceMatrix2;
    }

}

int 
main() {
    int nSequences = 5;
    int maxSequence = 11;

    char **sequences = (char**)malloc(nSequences * sizeof(char*));
    for (int i = 0; i < nSequences; i++) {
        sequences[i] = (char*)malloc((maxSequence + 1) * sizeof(char));
    }

    strcpy(sequences[0], "ATTGCCATT");
    strcpy(sequences[1], "ATGGCCATT");
    strcpy(sequences[2], "ATCCAATTTT");
    strcpy(sequences[3], "ATCTTCTT");
    strcpy(sequences[4], "ACTGACC");

    int size = nSequences;
    float **distanceMatrix = allocateMatrix(nSequences);
    //fillDistanceMatrix(nSequences, sequences, distanceMatrix);
    float values[5][5] = {
        {0, 5, 9, 9, 8},
        {5, 0, 10, 10, 9},
        {9, 10, 0, 8, 7},
        {9, 10, 8, 0, 3},
        {8, 9, 7, 3, 0}
    };

    for (int i = 0; i < nSequences; i++) {
        for (int j = 0; j < nSequences; j++) {
            distanceMatrix[i][j] = values[i][j];
        }
    }

    // printDistanceMatrix(nSequences, distanceMatrix);
    neighborJoining(nSequences, distanceMatrix);

    return 0;
}

