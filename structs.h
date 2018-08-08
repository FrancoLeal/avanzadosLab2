#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct node{
	int value;
	struct node * father;
	struct node * leftSon;
	struct node * rightBrother;
}Node;

typedef struct tree{
	Node * root;
} Tree;

typedef struct adj{
	int n;
	int * adjacents;
}Adjacents;

typedef struct path{
	int * path;
	int n;
	int dist;
} Path;

typedef struct graphMatrix{
	int ** matrix;
	int n;
} GraphMatrix;

Path * finalPath;

#endif