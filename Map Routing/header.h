#include <stdio.h>
#include <stdlib.h>

struct adjacentList
{
  int val;
  struct adjacentList * next;
};

typedef struct mapVertex
{
  int path;
  int x;
  int y;
  struct adjacentList * adjList;
  int pred;
  int eucliddist;
}Vertex;

void isdone(Vertex * mapVertex, int numVertices);
int ele(int numVertices, int visited, Vertex * mapVertex, int path);
void swap(int A, int B, Vertex * mapVertex);
void swap_address(int A, int B, int * address);
void downheaptraverse(Vertex * mapVertex, int * address, int location, int Minhp);
void upheaptraverse(Vertex * mapVertex, int * address, int location);
int map(FILE * map, Vertex * mapVertex, int * address, int numVertices, int numEdges);
void printmap(Vertex * mapVertex, int * address, int destination);
void djikstra_shortest(int source, int destination, Vertex * mapVertex, int * address, int numVertices);
void funcfree(Vertex * mapVertex, int numVertices);








