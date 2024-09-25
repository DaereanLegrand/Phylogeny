#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int is_cluster;
    int size;
    union {
        int value;
        struct {
            int length[2];
            struct Node *child[2];
        };
    };
} Node;

Node* create_value_node(int);
Node* create_cluster_node(Node*, Node*);
Node** resize_clusters(Node**, int*, int);
void printNode(Node*);
void printCluster(int, Node**);

#endif
