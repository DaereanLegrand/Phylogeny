#include "node.h"
#include <float.h>

Node* 
create_value_node(int value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->is_cluster = 0;
    new_node->size = 1;
    new_node->value = value;
    return new_node;
}

Node* 
create_cluster_node(Node *l, Node *r) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->is_cluster = 1;
    new_node->size = l->size + r->size;
    new_node->child[0] = l;
    new_node->child[1] = r;
    new_node->length[0] = FLT_MAX;
    new_node->length[1] = FLT_MAX;
    return new_node;
}

Node** 
resize_clusters(Node **clusters, int *n, int index_to_remove) {
    for (int i = index_to_remove; i < (*n) - 1; i++) {
        clusters[i] = clusters[i + 1];
    }
    (*n)--;
    clusters = (Node**)realloc(clusters, (*n) * sizeof(Node*));
    return clusters;
}

void 
printNode(Node* node) {
    if (node->is_cluster) {
        printf("(");
        printNode(node->child[0]);
        printf(", ");
        printNode(node->child[1]);
        printf(")");
    } else {
        printf("%d", node->value);
    }
}

void 
printCluster(int sz, Node **clusters) {
    printf("[");
    for (int i = 0; i < sz; i++) {
        printNode(clusters[i]);
        if (i < sz - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


void
printNodeASCII(Node* node, int depth, int is_left) {
    if (node == NULL) return;
    
    if (depth > 0) {
        printf("%*s", 4 * depth, " ");
        if (is_left) {
            printf("├── ");
        } else {
            printf("└── ");
        }
    }
    
    if (node->is_cluster) {
        printf("Cluster");
        if (node->length[0] != FLT_MAX || node->length[1] != FLT_MAX) {
            printf(" (%.2f, %.2f)", node->length[0], node->length[1]);
        }
        printf("\n");
        printNodeASCII(node->child[0], depth + 1, 1);
        printNodeASCII(node->child[1], depth + 1, 0);
    } else {
        printf("%d\n", node->value);
    }
}

void
printNodeASCIIfromArray(Node* node, int depth, int is_left, int nstrings, char **strings) {
    if (node == NULL) return;
    
    if (depth > 0) {
        printf("%*s", 4 * depth, " ");
        if (is_left) {
            printf("├── ");
        } else {
            printf("└── ");
        }
    }
    
    if (node->is_cluster) {
        printf("Cluster");
        if (node->length[0] != FLT_MAX || node->length[1] != FLT_MAX) {
            printf(" (%.2f, %.2f)", node->length[0], node->length[1]);
        }
        printf("\n");
        printNodeASCIIfromArray(node->child[0], depth + 1, 1, nstrings, strings);
        printNodeASCIIfromArray(node->child[1], depth + 1, 0, nstrings, strings);
    } else {
        if (nstrings > node->value && node->value >= 0) {
            printf("{%s}\n", strings[node->value]);
        }
    }
}

void 
printCluster2(int sz, Node **clusters) {
    printCluster(sz, clusters);
    for (int i = 0; i < sz; i++) {
        printf("Cluster %d:\n", i + 1);
        printNodeASCII(clusters[i], 0, 0);
    }
}

void 
printCluster3(int sz, Node **clusters, int nstrings, char **strings) {
    printCluster(sz, clusters);
    for (int i = 0; i < sz; i++) {
        printf("Cluster %d:\n", i + 1);
        printNodeASCIIfromArray(clusters[i], 0, 0, nstrings, strings);
    }
}
