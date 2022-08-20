#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <klee/klee.h>
#include<assert.h>

// Implementation of a binary search tree
// The tree has distinct positive keys only
// Removed print statements in this version to increase efficiency

#define SIZE 6
#define TREE_SIZE 5

typedef struct treeNode{
    int32_t key;
    int32_t parentIndex;
    int32_t leftChildIndex; 
    int32_t rightChildIndex; 
}treeNode_t;

typedef struct tree{
    int32_t rootIndex;
    treeNode_t arrNodes[TREE_SIZE]; // tree only takes 5 nodes, will try to insert more
}tree_t;

int32_t treeMinimum(tree_t tree){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        //fprintf(stderr, "Tree is empty!");
        return -1;
    }
    while(tree.arrNodes[current_node_index].leftChildIndex != -1){
        current_node_index = tree.arrNodes[current_node_index].leftChildIndex;
    }
    return current_node_index;
}

int32_t treeMaximum(tree_t tree){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        //fprintf(stderr, "Tree is empty!");
        return -1;
    }
    while(tree.arrNodes[current_node_index].rightChildIndex != -1){
        current_node_index = tree.arrNodes[current_node_index].rightChildIndex;
    }
    return current_node_index;
}

int32_t treeSearch(tree_t tree, int32_t key){ // DONE
    int32_t current_node_index = tree.rootIndex;
    if(current_node_index < 0 || current_node_index >= sizeof(tree.arrNodes)){
        //fprintf(stderr, "Tree is empty!");
        return -1;
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
        //fprintf(stderr, "Index of node to find successor is out of range!\n");
        return -1;
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
        //fprintf(stderr, "Cannot insert node with negative key!\n");
        return;
    }
    if(treeSearch(*tree, node.key) != -1){
        //fprintf(stderr, "Key is already in the tree, duplicate keys are not allowed!\n");
        return;
    }
    int32_t node_index = -1;
    // Find a place to insert the node
    for(int i = 0; i < (sizeof(tree->arrNodes)/sizeof(tree->arrNodes[0])); i++){ // Had a bug, i was between 0 and size of array in bytes, it should be between 0 and number of array elements
        if(tree->arrNodes[i].key == -1){
            tree->arrNodes[i] = node;
            node_index = i;
            break;
        }
    }
    if(node_index == -1){
        //fprintf(stderr, "Memory is full, cannot insert more nodes in tree!\n");
        return;
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
        //fprintf(stderr, "Node of index to delete is out of range!\n");
        return;
    }
    if(tree->arrNodes[node_index].key == -1){
        //fprintf(stderr, "No node exists at this index to delete!\n");
        return;
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
    
    // Assert that treeSearch finds all elements that have been inserted,
    // if the key is positive, it must have been inserted in the tree
    // ### buggy specification given above, correct spec: if the key is positive AND there was space in the tree, then the element was inserted in the tree and can be found by a search
    int tree_size_count = 0;
    for(int i = 0; i < SIZE; i++){
        if(symbolic_array[i] >= 0){
            if(tree_size_count < TREE_SIZE){
                assert(treeSearch(main_tree, symbolic_array[i]) != -1);
                tree_size_count += 1;
            }
        }
        else{ // if the key is negative, assert that it wasn't entered in the tree
            assert(treeSearch(main_tree, symbolic_array[i]) == -1);
        }
    }

    int32_t indexMax = treeMaximum(main_tree);
    int32_t tempIndex;

    // Assert that treeMaximum function returns the index of the node with the max key in the tree
    if(indexMax != -1){ // If tree is not empty
        for(int i = 0; i < SIZE; i++){
            tempIndex = treeSearch(main_tree, symbolic_array[i]);
            if(tempIndex != -1){ // If key was actually inserted in tree
                assert(main_tree.arrNodes[indexMax].key >= main_tree.arrNodes[tempIndex].key);
            }
        }
    }

    int32_t indexMin = treeMinimum(main_tree);
    // Assert that treeMinimum function returns the index of the node with the kmin key in the tree
    if(indexMin != -1){ // If tree is not empty
        for(int i = 0; i < SIZE; i++){
            tempIndex = treeSearch(main_tree, symbolic_array[i]);
            if(tempIndex != -1){ // If key was actually inserted in tree
                assert(main_tree.arrNodes[indexMin].key <= main_tree.arrNodes[tempIndex].key);
            }
        }
    }

    int32_t succIndex;
    int32_t origIndex;

    // Assert that treeSuccessor returns the successor, here testing on the last element of the symbolic array
    origIndex = treeSearch(main_tree, tempNode.key); // index of last element in symbolic array, we will search for its successor in the tree
    if(origIndex != -1){ // if the node is in the tree
        succIndex = treeSuccessor(main_tree, tempIndex); // search for its successor
        if(succIndex != -1){ // if there is a successor
            for(int i = 0; i < SIZE; i++){
                tempIndex = treeSearch(main_tree, symbolic_array[i]); 
                if(tempIndex != -1){ // If the value in the current array position was inserted in the tree
                    if(main_tree.arrNodes[tempIndex].key != tempNode.key && tempIndex != succIndex){ // no key is less than successor AND larger than original key
                        assert(!(main_tree.arrNodes[tempIndex].key < main_tree.arrNodes[succIndex].key && main_tree.arrNodes[tempIndex].key > main_tree.arrNodes[origIndex].key));
                    }
                }
            }
        }
        else{ // assert there is no successor
            for(int i = 0; i < SIZE; i++){
                tempIndex = treeSearch(main_tree, symbolic_array[i]);
                if(tempIndex != -1){ // if symbolic element was used to create a node in the tree
                    assert(tempNode.key >= main_tree.arrNodes[tempIndex].key); // if there is no successor, the original key is the max key
                }
            }
        }
    }

    // assert behaviour of inorder tree walk
    // TODO


    for(int i = 0 ; i < SIZE; i++){
        treeDeleteNode(&main_tree, treeMaximum(main_tree));
    }

    return 0;
}



