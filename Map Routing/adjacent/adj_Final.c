#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adj_header.h"
#define MAXVERTICES 10000
#define MAXEDGES 100000


int main(int argc, char ** argv)
{
  int ** mapedge;
  int mapvertices;
  int mapedges;

  mapedge = getmap(argv[1], &mapvertices, &mapedges);
  print(mapvertices, mapedges, mapedge);

  return EXIT_SUCCESS;
}

int ** getmap (char * inFile, int * mapvertices, int * mapedges)
{
  FILE * infptr;
  infptr = fopen(inFile, "rb");
  if (infptr == NULL)
  {
    return NULL;
  }

mapvertex begin;
int ** mapedge;
int i = 0;

fscanf(infptr, "%d %d", mapvertices, mapedges);
mapedge = malloc ((*mapedges) * sizeof(int *));

for (i = 0; i <= *mapedges; i++)
{
  mapedge[i] = malloc(2 * sizeof(int));
}
begin = malloc(sizeof(mapvertex) ** mapvertices);
readvertex(infptr, begin, mapvertices);
readedge(infptr, mapedge, mapedges);
return mapedge;
}


void readvertex(FILE * infptr, mapvertex begin, int * mapvertices)
{
  int i = 0;
  for (i =0; i < *mapvertices; i++)
  {
    fscanf(infptr, "%d %d %d", &(begin ->currnode), &(begin -> x), &(begin -> y));
  }

}

void readedge(FILE *infptr, int ** mapedge, int * mapedges)
{
  int i = 0;

  for (i =0; i < *mapedges; i++)
  {
    fscanf(infptr, "%d %d", &mapedge[i][0], &mapedge[i][1]);
  }

}

void print(int mapvertices, int mapedges, int ** mapedge)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < mapvertices; i++)
  {
    printf("%d:",i);
    for (j = 0; j < mapedges; j++)
    {
      if (i == mapedge[j][0])
      {
        printf(" %d",mapedge[j][1]);
      }
      if (i == mapedge[j][1])
      {
        printf(" %d",mapedge[j][0]);
      }
    }
    printf("\n");
  }
}