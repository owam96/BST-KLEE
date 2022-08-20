#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <klee/klee.h>

// Implementation of a binary search tree
// For now, code assumes distinct keys

#define SIZE 5

typedef struct treeNode{
    int32_t key;
    int32_t parentIndex;
    int32_t leftChildIndex; 
    int32_t rightChildIndex; 
}treeNode_t;

typedef struct tree{
    int32_t rootIndex;
    treeNode_t arrNodes[40];
}tree_t;

int32_t treeMinimum(tree_t tree){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        fprintf(stderr, "Tree is empty!");
        exit(-1);
    }
    while(tree.arrNodes[current_node_index].leftChildIndex != -1){
        current_node_index = tree.arrNodes[current_node_index].leftChildIndex;
    }
    return current_node_index;
}

int32_t treeMaximum(tree_t tree){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        fprintf(stderr, "Tree is empty!");
        exit(-1);
    }
    while(tree.arrNodes[current_node_index].rightChildIndex != -1){
        current_node_index = tree.arrNodes[current_node_index].rightChildIndex;
    }
    return current_node_index;
}

int32_t treeSearch(tree_t tree, int32_t key){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        fprintf(stderr, "Tree is empty!");
        exit(-1);
    }
    while(current_node_index != -1 && key != tree.arrNodes[current_node_index].key){
        if(key < tree.arrNodes[current_node_index].key){
            current_node_index = tree.arrNodes[current_node_index].leftChildIndex;
        }
        else{
            current_node_index = tree.arrNodes[current_node_index].rightChildIndex;
        }
    }
    return current_node_index;
}

int32_t treeSuccessor(tree_t tree, int32_t node_index){ // DONE
    if(node_index < 0 || node_index >= sizeof(tree.arrNodes)){
        fprintf(stderr, "Index of node to find successor is out of range!\n");
        exit(-1);
    }
    if(tree.arrNodes[node_index].rightChildIndex != -1){
        tree_t new_tree = tree;
        new_tree.rootIndex = tree.arrNodes[node_index].rightChildIndex;
        return treeMinimum(new_tree);
    }
    int32_t current_parent_index = tree.arrNodes[node_index].parentIndex;
    while(current_parent_index != -1 && node_index == tree.arrNodes[current_parent_index].rightChildIndex){ 
        node_index = current_parent_index;
        current_parent_index = tree.arrNodes[current_parent_index].parentIndex;
    }
    return current_parent_index;
}

void inorderTreeWalk(tree_t tree){ // DONE
    tree_t tree1 = tree;
    tree_t tree2 = tree;
    if(tree.rootIndex != -1){
        tree1.rootIndex = tree.arrNodes[tree.rootIndex].leftChildIndex;
        tree2.rootIndex = tree.arrNodes[tree.rootIndex].rightChildIndex;
        inorderTreeWalk(tree1);
        printf("%d ", tree.arrNodes[tree.rootIndex].key);
        inorderTreeWalk(tree2);
    }
    return;
}

void treeInsertNode(tree_t* tree, treeNode_t node){ // DONE
    if(node.key < 0){
        fprintf(stderr, "Cannot insert node with negative key!\n");
        exit(-1);
    }
    int32_t node_index = -1;
    // Find a place to insert the node
    for(int i = 0; i < sizeof(tree->arrNodes); i++){
        if(tree->arrNodes[i].key == -1){
            tree->arrNodes[i] = node;
            node_index = i;
            break;
        }
    }
    if(node_index == -1){
        fprintf(stderr, "Memory is full, cannot insert more nodes in tree!\n");
        exit(-1);
    }
    int32_t current_parent_index = -1;
    int32_t current_node_index = tree->rootIndex;
    while(current_node_index != -1){
        current_parent_index = current_node_index;
        if(node.key < tree->arrNodes[current_node_index].key){
            current_node_index = tree->arrNodes[current_node_index].leftChildIndex;
        }
        else{
            current_node_index = tree->arrNodes[current_node_index].rightChildIndex;
        }
    }
    tree->arrNodes[node_index].parentIndex = current_parent_index;
    if(current_parent_index == -1){
        tree->rootIndex = node_index;
    }
    else{
        if(node.key < tree->arrNodes[current_parent_index].key){
            tree->arrNodes[current_parent_index].leftChildIndex = node_index;
        }
        else{
            tree->arrNodes[current_parent_index].rightChildIndex = node_index;
        }
    }
}

void treeDeleteNode(tree_t* tree, int32_t node_index){
    int32_t y;
    int32_t x;
    if(node_index < 0 || node_index >= sizeof(tree->arrNodes)){
        fprintf(stderr, "Node of index to delete is out of range!\n");
        exit(-1);
    }
    if(tree->arrNodes[node_index].key == -1){
        fprintf(stderr, "No node exists at this index to delete!\n");
        exit(-1);
    }
    if(tree->arrNodes[node_index].leftChildIndex == -1 || tree->arrNodes[node_index].rightChildIndex == -1){
        y = node_index;
    }
    else{
        y = treeSuccessor(*tree, node_index);
    }
    if(tree->arrNodes[y].leftChildIndex != -1){
        x = tree->arrNodes[y].leftChildIndex;
    }
    else{
        x = tree->arrNodes[y].rightChildIndex;
    }
    if(x != -1){
        tree->arrNodes[x].parentIndex = tree->arrNodes[y].parentIndex;
    }
    if(tree->arrNodes[y].parentIndex == -1){
        tree->rootIndex = x;
    }
    else{
        if(y == tree->arrNodes[tree->arrNodes[y].parentIndex].leftChildIndex){
            tree->arrNodes[tree->arrNodes[y].parentIndex].leftChildIndex = x;
        }
        else{
            tree->arrNodes[tree->arrNodes[y].parentIndex].rightChildIndex = x;
        }
    }
    if(y != node_index){
        tree->arrNodes[node_index].key = tree->arrNodes[y].key;
        tree->arrNodes[y].key = -1;
        tree->arrNodes[y].leftChildIndex = -1;
        tree->arrNodes[y].parentIndex = -1;
        tree->arrNodes[y].rightChildIndex = -1;
    }
    else{
        tree->arrNodes[node_index].key = -1;
        tree->arrNodes[node_index].leftChildIndex = -1;
        tree->arrNodes[node_index].parentIndex = -1;
        tree->arrNodes[node_index].rightChildIndex = -1;
    }
}

int main(){
    tree_t main_tree;
    treeNode_t tempNode;
    int32_t symbolic_array[SIZE];

    klee_make_symbolic(symbolic_array, sizeof(symbolic_array), "sym_arr");

    for(int i = 0; i < SIZE; i++){
            klee_assume(symbolic_array[i] > 31);
            klee_assume(symbolic_array[i] < 127);
    }

    // Initialize everything to -1
    main_tree.rootIndex = -1;
    memset(main_tree.arrNodes, -1, sizeof(main_tree.arrNodes));
    tempNode.key = -1;
    tempNode.leftChildIndex = -1;
    tempNode.parentIndex = -1;
    tempNode.rightChildIndex = -1;

    for(int i = 0 ; i < SIZE; i++){
            tempNode.key = symbolic_array[i];
            treeInsertNode(&main_tree, tempNode);
    }
    
    treeMaximum(main_tree);
    treeMinimum(main_tree);

    int32_t temp_index;
    treeSearch(main_tree, tempNode.key);
    temp_index = treeSearch(main_tree, tempNode.key);
    treeSuccessor(main_tree, temp_index);

    
    for(int i = 0 ; i < SIZE; i++){
        treeDeleteNode(&main_tree, treeMaximum(main_tree));
    }

    return 0;
}



