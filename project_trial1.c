#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Implementation of a binary search tree

typedef struct treeNode{
    uint32_t key;
    struct treeNode* ptr_parent;
    struct treeNode* ptr_left_child; 
    struct treeNode* ptr_right_child;
}treeNode;

treeNode* createTreeNode(uint32_t key){
    treeNode* newNode = (treeNode*) malloc(sizeof(treeNode));
    if (newNode == NULL){
        fprintf(stderr, "Memory allocation failed!\n");
        exit(-1);
    }
    newNode->key = key;
    return newNode;
}

treeNode* treeMinimum(treeNode* ptr_tree_root){
    treeNode* ptr_current_node = ptr_tree_root;
    while(ptr_current_node->ptr_left_child != NULL){
        ptr_current_node = ptr_current_node->ptr_left_child;
    }
    return ptr_current_node;
}

treeNode* treeMaximum(treeNode* ptr_tree_root){
    treeNode* ptr_current_node = ptr_tree_root;
    while(ptr_current_node->ptr_right_child != NULL){
        ptr_current_node = ptr_current_node->ptr_right_child;
    }
    return ptr_current_node;
}

treeNode* treeSuccessor(treeNode* ptr_node_in_tree){
    if(ptr_node_in_tree->ptr_right_child != NULL){
        return treeMinimum(ptr_node_in_tree->ptr_right_child);
    }
    treeNode* ptr_current_parent = ptr_node_in_tree->ptr_parent;
    while(ptr_current_parent != NULL && ptr_node_in_tree == ptr_current_parent->ptr_right_child){
        ptr_node_in_tree = ptr_current_parent;
        ptr_current_parent = ptr_current_parent->ptr_parent;
    }
    return ptr_current_parent;
}

treeNode* treeInsertNode(treeNode* ptr_tree_root, treeNode* ptr_node){
    if(ptr_tree_root->ptr_parent != NULL){
        fprintf(stderr, "Not tree root!\n");
        return ptr_tree_root;
    }
    treeNode* ptr_current_parent = NULL;
    treeNode* ptr_current_node = ptr_tree_root;
    while(ptr_current_node != NULL){
        ptr_current_parent = ptr_current_node;
        if(ptr_node->key < ptr_current_node->key){
            ptr_current_node = ptr_current_node->ptr_left_child;
        }
        else{
            ptr_current_node = ptr_current_node->ptr_right_child;
        }
    }
    ptr_node->ptr_parent = ptr_current_parent;
    if(ptr_current_parent == NULL){
        ptr_tree_root = ptr_node;
    }
    else{
        if(ptr_node->key < ptr_current_parent->key){
        ptr_current_parent->ptr_left_child = ptr_node;
        }
        else{
            ptr_current_parent->ptr_right_child = ptr_node;
        }
    }
    return ptr_tree_root;
}

treeNode* treeDeleteNode(treeNode* ptr_tree_root, treeNode* ptr_node){
    treeNode* y;
    treeNode* x;
    if(ptr_node->ptr_left_child == NULL || ptr_node->ptr_right_child == NULL){
        y = ptr_node;
    }
    else{
        y = treeSuccessor(ptr_node);
    }
    if(y->ptr_left_child != NULL){
        x = y->ptr_left_child;
    }
    else{
        x = y->ptr_right_child;
    }
    if(x != NULL){
        x->ptr_parent = y->ptr_parent;
    }
    if(y->ptr_parent == NULL){
        ptr_tree_root = x;
    }
    else{
        if(y == y->ptr_parent->ptr_left_child){
            y->ptr_parent->ptr_left_child = x;
        }
        else{
            y->ptr_parent->ptr_right_child = x;
        }
    }
    if(y != ptr_node){
        ptr_node->key = y->key;
        free(y);
    }
    else{
        free(ptr_node);
    }
    return ptr_tree_root;
}

treeNode* treeSearch(treeNode* ptr_tree_root, uint32_t key){
    treeNode* ptr_current_node = ptr_tree_root;
    while(ptr_current_node != NULL && key != ptr_current_node->key){
        if(key < ptr_current_node->key){
            ptr_current_node = ptr_current_node->ptr_left_child;
        }
        else{
            ptr_current_node = ptr_current_node->ptr_right_child;
        }
    }
    return ptr_current_node;
}

void inorderTreeWalk(treeNode* ptr_tree_root){
    if(ptr_tree_root != NULL){
        inorderTreeWalk(ptr_tree_root->ptr_left_child);
        printf("%d", ptr_tree_root->key);
        inorderTreeWalk(ptr_tree_root->ptr_right_child);
    }
    return;
}

int main(int argc, char** argv){
    treeNode* treeRoot = NULL;
    treeNode* tempNode;
    uint32_t tempKey;

    if(argc > 1){
        for(int i = 1 ; i < argc; i++){
            tempKey = (uint32_t) atoi(argv[i]);
            tempNode = createTreeNode(tempKey);
            treeRoot = treeInsertNode(treeRoot, tempNode);
        }
    }

    inorderTreeWalk(treeRoot);

    return 0;
}



