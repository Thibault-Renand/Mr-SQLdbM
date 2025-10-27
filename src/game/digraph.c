
#include "digraph.h"


TreeNode* create_node(Board state) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node) {
        perror("Erreur d'allocation mémoire pour l'arbre");
        exit(EXIT_FAILURE);
    }
    node->state = state;
    node->value = 0;
    node->children = NULL;
    node->numChildren = 0;
    return node;
}


void add_child(TreeNode *parent, TreeNode *child) {
    parent->numChildren++;
    parent->children = (TreeNode **)realloc(parent->children, parent->numChildren * sizeof(TreeNode *));
    if (!parent->children) {
        perror("Erreur d'allocation mémoire pour les enfants");
        exit(EXIT_FAILURE);
    }
    parent->children[parent->numChildren - 1] = child;
}


void free_tree(TreeNode *node) {
    if (node) {
        for (int i = 0; i < node->numChildren; i++) {
            free_tree(node->children[i]);
        }
        free(node->children);
        node->children = NULL;
        free(node);
        node = NULL;
    } else {
        printf("Aucun arbre à détruire.\n");
    }
}
