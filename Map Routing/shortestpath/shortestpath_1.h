struct intList
{
  int value;
  struct intList * next;
};

typedef struct Vertex
{
  int path;
  int x;
  int y;
  struct intList *List;
  int prev;
  int eucledian;
}Vertice;
int map(FILE *fmap, Vertice *vertex, int*address, int Vertices, int Edges);
void djikstra(int source, int destination, Vertice * vertex, int * address, int Vertices);
void up_heap_traversal(Vertice * vertex, int *address, int location);
void down_heap_traversal(Vertice * vertex, int * address, int location, int Minheap);
int Comp(int Vertices, int visited, Vertice * vertex, int path);
void swap(int location1, int location2, Vertice * vertex);
void swap_Address(int location1, int location2, int * address);
void begin(Vertice * vertex, int Vertices);
void print(Vertice * vertex, int* address, int destination);
void Free(Vertice * vertex, int Vertices);








