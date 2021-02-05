#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

int main (int argc, char **argv)
{
  if (argc != 3)
    {
      printf("Invalid commands\n");
      return EXIT_FAILURE;
    }
  FILE *fptr = fopen(argv[1], "r");
  if (fptr == NULL)
    {
      printf("File not found\n");
      return 0;
    }
  FILE *fptr2 = fopen(argv[2], "r");
  if (fptr2 == NULL)
    {
      printf("Query file not found\n");
      fclose(fptr);
      return 0;
    }

  int Vertices = -1;
  int Edges = -1;
  int Queries = -1;
  int source;
  int destination;
  fscanf(fptr, "%d %d", &(Vertices), &(Edges));
  if (Vertices == -1)
    {
      fclose(fptr);
      fclose(fptr2);
      return 0;
    }

  if (Edges == -1)
    {
      fclose(fptr);
      fclose(fptr2);
      return 0;
    }

  Vertice * vertex = malloc(sizeof(Vertice)*Vertices);
  int * address = malloc(sizeof(int)*Vertices);
  if (!map(fptr, vertex, address, Vertices, Edges))
    {
      fclose(fptr);
      fclose(fptr2);
      Free(vertex, Vertices);
      free(address);
      return 0;
    }
  fclose(fptr);
  fscanf(fptr2, "%d", &Queries);
  if (Queries == -1)
    {
      fclose(fptr2);
      Free(vertex, Vertices);
      free(address);
      return 0;
    }
  
  while (Queries-- > 0)
    {
      fscanf(fptr2, "%d %d", &source, &destination);
      djikstra(source, destination, vertex, address, Vertices);
      printf("%d\n", vertex[address[destination]].eucledian);
      print(vertex, address, destination);
      printf("\n");
      begin(vertex, Vertices);
    }
  fclose(fptr2);
  Free(vertex, Vertices);
  free(address);
  return 0;
}


void begin (Vertice * vertex, int Vertices)
{
  int i;
  for (i = 0; i < Vertices; i++)
    {
      vertex[i].eucledian = -1;
      vertex[i].prev = -1;
    }
}


int Comp(int Vertices, int visited, Vertice * vertex, int path)
{
  int first = Vertices - 1;
  int last = Vertices - visited;
  int mid = (first + last) / 2;
  while(last <= first)
    {
      mid = (first + last) / 2;
      if(vertex[mid].path == path)
	{
	  return 1;
	}
      else if(vertex[mid].path > path)
	{
	  first = mid - 1;
	}
      else
	{
	  last = mid + 1;
	}
    }
  return 0;
}


void swap(int location1, int location2, Vertice * vertex)
{
  Vertice t;
  t.x = vertex[location1].x;
  t.y = vertex[location1].y;
  t.path = vertex[location1].path;
  t.List = vertex[location1].List;
  t.prev = vertex[location1].prev;
  t.eucledian = vertex[location1].eucledian;
  
  vertex[location1].x = vertex[location2].x;
  vertex[location1].y = vertex[location2].y;
  vertex[location1].path = vertex[location2].path;
  vertex[location1].List = vertex[location2].List;
  vertex[location1].prev = vertex[location2].prev;
  vertex[location1].eucledian = vertex[location2].eucledian;

  vertex[location2].x = t.x;
  vertex[location2].y = t.y;
  vertex[location2].path = t.path;
  vertex[location2].List = t.List;
  vertex[location2].prev = t.prev;
  vertex[location2].eucledian = t.eucledian;
  
}


void swap_Address(int location1, int location2, int * address)
{
  int t;
  t = address[location1];
  address[location1] = address[location2];
  address[location2] = t;
}


void down_heap_traversal(Vertice * vertex, int * address, int location, int Minheap)
{
  while(location < Minheap)
    {
      if((location*2) + 1 >= Minheap)
	{
	  return;
	}
      if((location*2) + 2 >= Minheap)
	{
	  if(vertex[(location *2) + 1].eucledian == 1)
	    {
	      return;
	    }
	  if (vertex[location].eucledian == -1) 
	    {
	      swap(location, (location * 2) + 1, vertex);
	      swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
	    }
	  if (vertex[location].eucledian > vertex[(location * 2) + 1].eucledian)
	    {
	      swap(location, (location * 2) + 1, vertex);
	      swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
	    } 
	  return;
	}

      if (vertex[(location * 2) + 1].eucledian == -1 && vertex[(location * 2) + 2].eucledian == -1)
	{
	  return;
	}

      if(vertex[location].eucledian == -1)
	{
	  if(vertex[(location * 2) + 1].eucledian == -1)
	    {
	      swap(location, (location * 2) + 2, vertex);
	      swap_Address(vertex[location].path, vertex[(location * 2) + 2].path, address);
	      location = (location * 2) + 2;
	      continue;
	    }
	  else if(vertex[(location * 2) + 2].eucledian == -1)
	    {
	      swap(location, (location * 2) + 1, vertex);
	      swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
	      location = (location * 2) + 1;
	      continue;
	    }
	  else if(vertex[(location * 2) + 2].eucledian != -1 && vertex[(location * 2) + 2].eucledian != -1)
	    {
	      if (vertex[(location * 2) + 1].eucledian <= vertex[(location * 2) + 2].eucledian)
		{
		  swap(location, (location * 2) + 1, vertex);
		  swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
		  location = (location * 2) + 1;
		  continue;
		}
	      else
		{
		  swap(location, (location * 2) + 2, vertex);
		  swap_Address(vertex[location].path, vertex[(location * 2) + 2].path, address);
		  location = (location * 2) + 2;
		  continue;
		}
	    }
	}

      else
	{
	  if(vertex[(location * 2) + 1].eucledian == -1)
	    {
	      if(vertex[location].eucledian > vertex[(location * 2) + 2].eucledian)
		{
		  swap(location, (location * 2) + 2, vertex);
		  swap_Address(vertex[location].path, vertex[(location * 2) + 2].path, address);
		  location = (location * 2) + 2;
		  continue;
		}
	      else
		{
		  return;
		}
	    }
	  else if (vertex[(location * 2) + 2].eucledian == -1)
	    {
	      if(vertex[location].eucledian > vertex[(location * 2) + 1].eucledian)
		{
		  swap(location, (location * 2) + 1, vertex);
		  swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
		  location = (location * 2) + 1;
		  continue;
		}
	      else
		{
		  return;
		}
	    }
	  else if (vertex[(location * 2) + 2].eucledian != -1 && vertex[(location * 2) + 2].eucledian != -1)
	    {
	      if (vertex[location].eucledian <= vertex[(location * 2) + 1].eucledian && vertex[location].eucledian <= vertex[(location * 2) + 2].eucledian)
		{
		  return;
		}
	      else if (vertex[(location * 2) + 1].eucledian <= vertex[(location * 2) + 2].eucledian && vertex[location].eucledian > vertex[(location * 2) + 1].eucledian)
		{
		  swap(location, (location * 2) + 1, vertex);
		  swap_Address(vertex[location].path, vertex[(location * 2) + 1].path, address);
		  location = (location * 2) + 1;
		  continue;
		}
	      else if (vertex[(location * 2) + 1].eucledian > vertex[(location * 2) + 2].eucledian && vertex[location].eucledian > vertex[(location * 2) + 2].eucledian)
		 {
		   swap(location, (location * 2) + 2, vertex);
		   swap_Address(vertex[location].path, vertex[(location * 2) + 2].path, address);
		   location = (location * 2) + 2;
		   continue;
		 }
	    }
	}
    }
}


void up_heap_traversal(Vertice * vertex, int *address, int location)
{
  while ( location > 0)
    {
      if (vertex[location].eucledian == -1)
	{
	  return;
	}
      if (vertex[(location - 1)/2].eucledian != -1 && vertex[location].eucledian >= vertex[(location - 1)/2].eucledian)
	{
	  return;
	}
      swap(location, (location - 1)/2, vertex);
      swap_Address(vertex[location].path, vertex[(location - 1)/2].path, address);
      location = (location - 1)/2;
    }
}

       

int map(FILE *fmap, Vertice *vertex, int*address, int Vertices, int Edges)
{
  if(vertex == NULL)       
    {
      return 0;
    }
  if (address == NULL)
    {
      return 0;
    }
  if (Vertices <= 0)
    {
      return 0;
    }
  int i = 0;
  int src;
  int dest;
  for (i=0; i<Vertices;i++)
    {
      address[i] = i;
      fscanf(fmap, "%d %d %d", &(vertex[i].path), &(vertex[i].x), &(vertex[i].y));
      vertex[i].List = NULL;
      vertex[i].eucledian --;
      vertex[i].prev --;
    }

  for (i = 0; i < Edges; i++)
    {
      fscanf(fmap, "%d %d", &src, &dest);
      struct intList * A = malloc(sizeof(struct intList) * 1);
      if (A == NULL)
	{
	  return 0;
	}
      A -> value = dest;
      A -> next = vertex[src].List;
      vertex[src].List = A;
      struct intList *B = malloc(sizeof(struct intList) * 1);
      if (B == NULL)
	{
	  return 0;
	}
      B -> value = src;
      B -> next = vertex[dest].List;
      vertex[dest].List = B;
    }
  return 1;
}


void print(Vertice * vertex, int* address, int destination)
{
  if (vertex[address[destination]].prev == -1)
    {
      printf("%d ", vertex[address[destination]].path);
      return;
    }
  print(vertex,address,vertex[address[destination]].prev);
  printf("%d ", vertex[address[destination]].path);
}


void djikstra(int source, int destination, Vertice * vertex, int * address, int Vertices)
{
  int Minheap = Vertices;
  int visited = 0;
  vertex[address[source]].eucledian = 0;
  swap(0, address[source], vertex);
  swap_Address(vertex[address[source]].path, source, address);
  while (Minheap > 0)
    {
      if (vertex[0].path == destination)
	{
	  return;
	}
      Vertice popped = vertex[0];
      swap(0, Minheap - 1, vertex);
      swap_Address(vertex[0].path, vertex[Minheap - 1].path, address);
      Minheap --;
      visited ++;
      down_heap_traversal(vertex, address, 0, Minheap);
      struct intList * p;
      p = popped.List;
      while(p != NULL)
	{
	  if(Comp(Vertices, visited, vertex, p -> value))
	    {
	      p = p -> next;
	      continue;
	    }
	  int euc = sqrt((vertex[address[p -> value]].x - popped.x) * (vertex[address[p -> value]].x - popped.x) + (vertex[address[p -> value]].y - popped.y) * (vertex[address[p -> value]].y - popped.y));
	  if (vertex[address[p -> value]].eucledian != -1 && vertex[address[p->value]].eucledian <= popped.eucledian + euc)
	    {
	      p = p -> next;
	      continue;
	    }
	  vertex[address[p -> value]].eucledian = popped.eucledian + euc;
	  vertex[address[p -> value]].prev = popped.path;
	  up_heap_traversal(vertex, address, address[p -> value]);
	  p = p -> next;
	}
    }
}


void Free(Vertice * vertex, int Vertices)
{
  if (vertex == NULL)
    {
      return;
    }
  int i;
  struct intList * p;
  struct intList * q;
  for(i = 0; i < Vertices; i++)
    {
      p = vertex[i].List;
      while(p != NULL)
	{
	  q = p;
	  p = p -> next;
	  free(q);
	}
    }
  free(vertex);
}


    
      
  
      
	    