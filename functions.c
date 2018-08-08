#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"
/*Incializa un nodo*/
Node * initNode(int value){
	Node * node = (Node*)malloc(sizeof(Node));
	node->value=value;
	node->father=NULL;
	node->leftSon = NULL;
	node->rightBrother=NULL;
	return node;
}
/*inicializa un arbol*/
Tree * initTree(){
	Node * node = NULL;
}
/*Iniciliza matriz*/
GraphMatrix * initMatrix(int n){
	GraphMatrix * matrix = (GraphMatrix*)malloc(sizeof(GraphMatrix));
	if(matrix==NULL){
		printf("Memoria insuficiente\n");
		return NULL;
	}
	int i=0,j=0;
	matrix->matrix = (int**)malloc(sizeof(int*)*n);
	if(matrix->matrix==NULL){
		printf("Memoria insuficiente\n");
		return NULL;
	}
	for(i=0;i<n;i++){
		matrix->matrix[i]=(int*)malloc(sizeof(int)*n);
		if(matrix->matrix[i]==NULL){
			printf("Memoria insuficiente\n");
			return NULL;
		}
	}
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			matrix->matrix[i][j]=-1;
		}
	}
	matrix->n=n;
	return matrix;
}
/*Abrir y retornar archivo de entrada*/
FILE * openFile(char * name){
	FILE * fp = fopen(name,"r");
	if(fp==NULL){
		printf("No se pudo abrir el archivo\n");
	}
	return fp;
}
/*Guardar artista y peso en matriz de adyacencia*/
void storeEdge(GraphMatrix * matrix, int nodeS, int nodeD, int dist){
	if(nodeD==21){
		nodeD=matrix->n-1;
	}
	if(nodeS==21){
		nodeS=matrix->n-1;
	}
	matrix->matrix[nodeS][nodeD]=dist;
}
/*Completar matiz de adyacencia con los datos del archivo*/
GraphMatrix * createMatrix(FILE * fp){
	int n,nodeS,nodeD,dist;
	fscanf(fp,"%d",&n);
	GraphMatrix * matrix = initMatrix(n+2);
	if(matrix==NULL){
		return matrix;
	}
	while(!feof(fp)){
		fscanf(fp,"%d %d %d",&nodeS,&nodeD,&dist);
		storeEdge(matrix,nodeS,nodeD,dist);
	}
	return matrix;
}

void printMatrix(GraphMatrix * matrix){
	int i,j;
	for(i=0;i<matrix->n;i++){
		for(j=0;j<matrix->n;j++){
			printf("[%d] ",matrix->matrix[i][j]);
		}
		printf("\n");
	}
}
/*Encuentra los nodos adyacentes de un nodo en el grafo y los retorna*/
Adjacents adjacents(GraphMatrix * matrix , int value){
	int i,count=0;
	Adjacents adj;
	for(i=0;i<matrix->n;i++){
		if(matrix->matrix[value][i]!=-1){
			count++;
		}
	}
	adj.adjacents = (int*)malloc(sizeof(int)*count);
	adj.n = count;
	count = 0;
	for(i=0;i<matrix->n;i++){
		if(matrix->matrix[value][i]!=-1){
			adj.adjacents[count]=i;
			count++;
		}
	}
	return adj;
}
/*Agregar hijo a un nodo del arbol*/
void addSon(int value, Node * node){
	if(node->leftSon==NULL){
		node->leftSon = initNode(value);
		node->leftSon->father=node;
	}
	else{
		Node * aux = node->leftSon;
		while(aux->rightBrother!=NULL){
			aux=aux->rightBrother;
		}
		aux->rightBrother = initNode(value);
		aux->rightBrother->father=node;
	}
}
/*Consulta si un nodo ya fue visitado en la creacion del camino. Solo se requiere hacer para todo los padres, hasta la raiz.
Retorna 1 si fue visitado y 0 en caso contrario*/
int wasVisited(Node * node, int value){
	Node * aux = node;
	while(aux->value!=0){
		if(aux->value==value){
			return 1;
		}
		aux=aux->father;
	}
	return 0;
}
/*Muestra por pantalla todos los nodos en una estrucutra de adyacentes*/
void printAdyacentes(Adjacents adj){
	int i;
	for(i=0;i<adj.n;i++){
		printf("%d ",adj.adjacents[i]);
	}
}
/*Agrega todos los nodos adyacentes a un nodo en el arbol*/
void completeAdjacents(GraphMatrix * matrix, Node * node){
	Adjacents adj = adjacents(matrix,node->value);
	//printAdyacentes(adj);
	int i;
	for(i=0;i<adj.n;i++){
		if(!wasVisited(node,adj.adjacents[i])){
			addSon(adj.adjacents[i],node);
		}
	}
}
/*llena el arbol de forma recursiva. Mientras el hijo izquierdo sea distinto de hoja y el hermano derecho no sea el ultimo*/
void completeTree(GraphMatrix * matrix, Node * node){
	completeAdjacents(matrix,node);
	if(node->leftSon!=NULL){
		completeTree(matrix,node->leftSon);
	}
	if(node->rightBrother!=NULL){
		completeTree(matrix,node->rightBrother);
	}	
}

void printSons(Node * node){
	node = node->leftSon;
	while(node!=NULL){
		printf("%d ",node->value);
		node=node->rightBrother;
	}
	printf("\n");
}

void printTree(Node * root){
	printSons(root);
	if(root->leftSon!=NULL){
		printTree(root->leftSon);
	}
	if(root->rightBrother!=NULL){
		printTree(root->rightBrother);
	}
}
/*Busca en la matriz de adyacencia el peso correspondiente a la arista (nodeS,NodeD)*/
int getEdgeDist(GraphMatrix * matrix, int nodeS, int nodeD){
	return matrix->matrix[nodeS][nodeD];
}
/*Agrega un nodo al arreglo que corresponde al camino que se desea encontrar. 
Devuelve el peso correspondiente a la arista compuesta por el nodo a agregar y el nodo anterior del arreglo
Si es el primer nodo agregado al arreglo, devuelve 0*/
int addNodePath(GraphMatrix * matrix, Path * path, Node * node, int pos){
	path->path[pos]=node->value;
	path->n++;
	if(pos==0){
		return 0;
	}
	else{
		return path->dist+=getEdgeDist(matrix,path->path[pos],path->path[pos-1]);
	}
}
/*Obtiene el camino desde una hoja hasta la raiz*/
Path * getPath(GraphMatrix * matrix, Node * node, int n){
	int i = 0;
	Path * path = (Path*)malloc(sizeof(Path));
	path->path=(int*)malloc(sizeof(int)*n);
	path->n=0;	
	path->dist = 0;
	Node * aux = node;
	while(aux!=NULL){
		addNodePath(matrix,path,aux,i);
		aux=aux->father;
		i++;
	}
	return path;
}
void printPath(Path * path){
	if(path==NULL){
		printf("Path null\n");
		return;
	}
	int i;
	for(i=0;i<path->n;i++){
		printf("%d",path->path[i]);
	}
	printf("  dist: %d\n",path->dist);
}
/*Copia un camino a una nueva variable y la retorna*/

Path * copyPath(Path * path){
	Path * aux = (Path*)malloc(sizeof(Path));
	path->n=path->n;
	path->dist=path->dist;
	int i;
	for(i=0;i<path->n;i++){
		aux->path[i]=path->path[i];
	}
	return aux;
}
/*Compara dos caminos y retorna el de menor peso*/
//ENTRADAS MODIFICADAS
Path * lowerDist(GraphMatrix * matrix, Path * path1, Path * path2){
	//printPath(path1);
	//printPath(path2);
	if(path1 ==NULL){
		if(path2 == NULL){
			return NULL;
		}
	//	printPath(path2);
		return path2;
	}
	if(path2==NULL){
	//	printPath(path1);
		return path1;
	}
	//printPath(path2);
	if(path1->n == 0 || path1->n < matrix->n){
		return path2;
	}
	if(path2->n == 0 || path2->n < matrix->n){
		return path1;
	}
	if(path1->dist < path2->dist){
	//	printPath(path1);
		return path1;
	}
	//printPath(path2);
	return path2;
}
/*Funcion principal. Funcion que recorre el arbol de manera recursiva y se encarga de obtener el camino cuando llega a una hoja. Luego, obtiene el camino y
guarda en finalPath el camino de menor peso encontrado*/
void getLower(GraphMatrix * matrix, Node * node){
	printf("Menor: ");
	printPath(finalPath);
	Node * aux = node;
	Path * path1 = NULL,* path2 = NULL, * path3 = NULL;
	if(aux->leftSon!=NULL){
		path1 = getPath(matrix,aux,matrix->n);
		printPath(path1);
		/*printf("*********PATH 1 ****************\n");
		printPath(path1);
		printf("***********FINAL PATH*********\n");
		printPath(finalPath);*/
		if(path1->dist > finalPath->dist && finalPath->dist !=0){
			printf("Camino no viable, dist %d\n",path1->dist);
		}
		else{
			getLower(matrix,aux->leftSon);
		}
	}
	else{
		path1 = getPath(matrix,aux,matrix->n);

	}
	if(aux->rightBrother!=NULL){
		path2 = getPath(matrix,aux,matrix->n);
		printPath(path2);
		/*printf("*********PATH 2****************\n");
		printPath(path2);
		printf("***********FINAL PATH*********\n");
		printPath(finalPath);*/
		if(path2->dist > finalPath->dist && finalPath->dist !=0){
			printf("Camino no viable, dist %d\n*******\n",path2->dist);
		}
		else{
			getLower(matrix,aux->rightBrother);
		}
	}
	else{
		path2 = getPath(matrix,aux,matrix->n);
	}
	/*printf("*******************OUT***********************\n");
	printf("*********PATH 1 ****************\n");
	printPath(path1);
	printf("*********PATH 2 ****************\n");
	printPath(path2);*/
	path3 = lowerDist(matrix,path1,path2);
	finalPath = lowerDist(matrix,finalPath,path3);
	/*printf("***********FINAL PATH*********\n");
	printPath(finalPath);*/
}
/*Escribe el archivo de salida con el camino de menor peso*/
void writeOutFile(Path * path){
	FILE * fp = fopen("salida.out","w");
	if(path==NULL){
		printf("Path null\n");
		return;
	}
	int i;
	for(i=path->n-1;i>0;i--){
		fprintf(fp,"%d - ",path->path[i]);
	}
	fprintf(fp,"21\n");
	if(path->dist >105){
		fprintf(fp,"Debe levantarse mas temprano\n");
	}
	fprintf(fp,"Duración: %d minutos\n",path->dist);
	fclose(fp);
}
/*Inicializador*/
void init(){
	FILE * fp = openFile("entrada.in");
	GraphMatrix * matrix = createMatrix(fp);
	printMatrix(matrix);
	Node * node = initNode(0);
	completeTree(matrix,node);
	//printTree(node);
	finalPath = (Path*)malloc(sizeof(Path));
	finalPath->n=0;
	finalPath->dist=0;
	getLower(matrix,node);
	//printPath(finalPath);
	writeOutFile(finalPath);
	fclose(fp);
}