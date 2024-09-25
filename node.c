#include "node.h"

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
