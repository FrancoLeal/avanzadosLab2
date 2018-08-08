#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include "structs.h"

Node * initNode(int value);

Tree * initTree();

GraphMatrix * initMatrix(int n);

FILE * openFile(char * name);

void storeEdge(GraphMatrix * matrix, int nodeS, int nodeD, int dist);

GraphMatrix * createMatrix(FILE * fp);

void printMatrix(GraphMatrix * matrix);

Adjacents adjacents(GraphMatrix * matrix , int value);

void addSon(int value, Node * node);

int wasVisited(Node * node, int value);

void printAdyacentes(Adjacents adj);

void completeAdjacents(GraphMatrix * matrix, Node * node);

void completeTree(GraphMatrix * matrix, Node * node);

void printSons(Node * node);

void printTree(Node * root);

int getEdgeDist(GraphMatrix * matrix, int nodeS, int nodeD);

int addNodePath(GraphMatrix * matrix, Path * path, Node * node, int pos);

Path * getPath(GraphMatrix * matrix, Node * node, int n);

void printPath(Path * path);

Path * copyPath(Path * path);

Path * lowerDist(GraphMatrix * matrix, Path * path1, Path * path2);

void getLower(GraphMatrix * matrix, Node * node);

void writeOutFile(Path * path);

void freePath(Path * path);

void freeTree(Node * node);

void init();

#endif