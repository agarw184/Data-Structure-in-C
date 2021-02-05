#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//data structure defintions 
typedef struct vertex
{
  int x;
  int y;
  int currnode;
} * mapvertex;

typedef struct edge 
{
  int first;
  int end;
} * mapedge;

//Function Defintions
int ** getmap (char * inFile, int * mapvertices, int * mapedges);
void readvertex(FILE* infptr, mapvertex begin, int * mapvertices);
void readedge(FILE * infptr, int ** mapedge, int * mapedges);
void print(int mapvertices, int mapedges, int ** mapedge);