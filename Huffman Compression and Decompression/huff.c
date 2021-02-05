#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMALPHA 256

//----------------------------------------------------------------------------------//
//Main function
//----------------------------------------------------------------------------------//
int main (int argc, char ** argv)
{
  if (argc != 2)
  {
    return EXIT_FAILURE;
  }
  char * outfile = malloc(sizeof(char)*(strlen(argv[1] + 6)));
  int i = 0;
  while(i < strlen(argv[1]))
  {
    *(outfile + i) = *(argv[1] + i);
    i++;
  }
  *(outfile + i + 0) = '.';
  *(outfile + i + 1) = 'h';
  *(outfile + i + 2) = 'u';
  *(outfile + i + 3) = 'f';
  *(outfile + i + 4) = 'f';
  *(outfile + i + 5) = '\0';
  encode(argv[1],outfile);
  free(outfile);
  return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------------//
//Encode 
//----------------------------------------------------------------------------------//
int encode(char* infile,char *outfile)
{
  unsigned int netsize = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int map[NUMALPHA];
  Freq  frequencies[NUMALPHA];
  bzero(frequencies, sizeof(Freq)*256);
  netsize = count(infile,frequencies);
  if (netsize == 0)
  {
    return EXIT_FAILURE;
  }
  sort(frequencies);
  lnode * head = Listbuild(frequencies);
  Headcheck(head);
  tnode * treeroot = CleanList(head);
  PrefixTreePrint(treeroot , 0);
  int numRow = tree_leaf(treeroot);
  int numCol = tree_height(treeroot);
  numCol++;

  int ** table = malloc(sizeof(int*) *numRow);
  for (i = 0; i < numRow; i++)
  {
    table[i] = malloc(sizeof(int) * (numCol));
    for(j = 0; j < numCol; j++)
    {
      table[i][j] = -1;
    }
  }

  Generatetable(treeroot,table);
  printtable(numRow,table);

  for (k = 0; k < NUMALPHA; k++)
  {
    map[k] = -1;
    int m;
    for (m = 0; m < numRow; m++)
    {
      if (table[m][0] == k)
      {
        map[k] = m; 
      }
    }
  }

  headeroftree(treeroot,netsize,outfile);
  compressionbitmanipulation(infile,outfile,table,map);

  for(i = 0; i < numRow; i++)
  {
    free(table[i]);
  }
  free(table);
  destroytree(treeroot);
  return 1;
}
//----------------------------------------------------------------------------------------//
//Compression Bit Manipulation 
//----------------------------------------------------------------------------------------//
int compressionbitmanipulation(char* infile,char* outfile, int** table, int* map)
{  
  unsigned char whichbit = 0;
  unsigned char curbyte = 0;
  unsigned char c;
  FILE * infilefptr = fopen(infile, "r");
  if (infilefptr == NULL)
    {
      return 0;
    }
  FILE * outfilefptr = fopen(outfile, "a");
  if (outfilefptr == NULL)
    {
      fclose(outfilefptr);
      return 0;
    }
  //fseek(infilefptr, 0, SEEK_SET);
  c = fgetc(infilefptr);
  while (!c)
    {
      int one = c;
      if (one != EOF)
      {
        int i = map[one];
        int j = 1;
        while (table[i][j] != -1)
        {
          writebit(outfilefptr,(table[i][j] = 1), &whichbit, &curbyte);
          j++;
        }
      }
      c = fgetc(infilefptr);
    }
    while (whichbit != 0)
    {
    	writebit(outfilefptr,0, &whichbit, &curbyte);
    }
    padZero(outfilefptr, &whichbit, &curbyte);
    fclose(infilefptr);
    fclose(outfilefptr);
    return 1;
}
//----------------------------------------------------------------------------------//
//Counts
//----------------------------------------------------------------------------------//
int count(char * infile, Freq* frequencies)
{
  FILE * fptr = fopen(infile, "r");
  int size = 0;
  int c = 0;

  if (fptr == NULL)
    {
      return EXIT_FAILURE;
    }
    while(!feof(fptr))
    {
      c = fgetc(fptr);
      if (c != EOF)
      {
        size++;
        frequencies[c].val = (unsigned char) c;
        frequencies[c].weight++;
      }
    }
    fclose(fptr);
    return size;
}

int compare(const void * p1, const void * p2)
{
  const Freq* pp1 = (const Freq *)p1;
  const Freq* pp2 = (const Freq *)p2;
  const int ppp1 = pp1 -> weight;
  const int ppp2 = pp2 -> weight;
  return (ppp1 - ppp2);
}

void sort(Freq* frequencies)
{
  qsort(frequencies, NUMALPHA, sizeof(Freq), compare);
}
//------------------------------------------------------------------------------------//
//List functions
//------------------------------------------------------------------------------------//
lnode *Listbuild(Freq * frequencies)
{
  lnode * head = NULL;
  int i = 0;
  while(frequencies[i].weight == 0)
  {
    i++;
  }
  if(i == NUMALPHA)
  {
    return NULL;
  }
  while(i < NUMALPHA)
  {

    tnode * treecurrnode = CreatesTree(frequencies[i].val,frequencies[i].weight);
    lnode * listcurrnode = CreatesListNode(treecurrnode);
    head = InsertintoList(head,listcurrnode);
    i++;
  }
    return head;
  }

lnode * InsertintoList(lnode *head, lnode* listcurrnode)
{
  if (head == NULL || listcurrnode->next != NULL)
  {
    return listcurrnode;
  }
  if(listcurrnode == NULL)
  {
    return head;
  }
  int freq1 = (head->ptr)->weight;
  int freq2 = (listcurrnode->ptr)->weight;
  if (freq1 > freq2)
  {
    listcurrnode->next = head;
    return listcurrnode;
  }
  head->next = InsertintoList(head->next,listcurrnode);
  return head;
}

lnode * CreatesListNode(tnode * treenode)
{
  lnode * currnode = malloc(sizeof(lnode));
  currnode -> next = NULL;
  currnode -> ptr = treenode;    
  return currnode;
}

void Headcheck(lnode* head)
{
    if(head == NULL)
  {
    return;
  }
}

tnode * CleanList(lnode *head){
  while ((head->next) != NULL)
    {
      head = Huffman(head);
    }
  tnode* root = head->ptr;
  free(head);
  return root;
}
//-------------------------------------------------------------------------------------------//
//Tree functions 
//--------------------------------------------------------------------------------------------//
tnode * CreatesTree(unsigned char val, int freqvalue)
{
  tnode * currnode = malloc(sizeof(tnode));
  currnode -> left = NULL;
  currnode -> right = NULL;
  currnode -> weight = freqvalue;
  currnode -> data = val;
  return currnode;
}

tnode * MergesTree(tnode * tn1, tnode * tn2)
{  
  tnode * Treenode = malloc(sizeof(tnode)); 
  Treenode -> left = tn1;
  Treenode -> right = tn2;
  Treenode -> data = 0;
  Treenode -> weight = ((tn1->weight)+(tn2->weight));
  return Treenode;
}

void destroytree(tnode* root)
{
  if (root == NULL)
  {
    return;
  }
  destroytree(root -> left);
  destroytree(root -> right);
  free(root);
  return;
}

void PrintSingleHelper(tnode *tn)
{
  printf("Char: %c, Freq: %d\n", tn->data, tn->weight);
}

void PrefixTreePrint(tnode *root , int level)
{
  if(root == NULL)
  {
   return ;
  }
  tnode * lc = root -> left ;
  tnode * rc = root -> right ; 
  PrintSingleHelper(root);
  PrefixTreePrint(lc, level + 1);
  PrefixTreePrint(rc, level + 1);
  return;
}
//--------------------------------------------------------------------------------------//
//Huffman
//-------------------------------------------------------------------------------------//
lnode * Huffman(lnode *head)
{  
  while (head -> next != NULL)
    {
      lnode * second = head -> next;
      lnode * third = second -> next;
      tnode * tn1 = head -> ptr;
      tnode * tn2 = second -> ptr;

      free(head);
      free(second);
      head = third;

      tnode * mergetree = MergesTree(tn1,tn2);
      lnode * newnode = CreatesListNode(mergetree);
      head = InsertintoList(head, newnode);
    }
  return head;
}
//------------------------------------------------------------------------------------//
//Get Height of tree
//------------------------------------------------------------------------------------//

int tree_height(tnode * tn)
{
  if(tn == NULL)
  {
    return 0;
  }
  int left = tree_height(tn->left);
  int right = tree_height(tn->right);
  if (left > right)
  {
    return(left + 1);
  }
  return(right + 1);
}
//----------------------------------------------------------------//
//leafheight
//---------------------------------------------------------------//
int tree_leaf(tnode * tn)
{
  if (tn == NULL)
  {
    return 0;
  }
  if(tn -> left == NULL && tn -> right ==NULL)
  {
    return 1;
  }
  return tree_leaf(tn -> left) + tree_leaf(tn -> right);
}
//-------------------------------------------------------------------------------------//
//Utility functions 
//------------------------------------------------------------------------------------//
int writebit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte)
{
  if ((*whichbit) == 0)
    {
      *curbyte = 0;
    }
  unsigned char tempchar = bit << (7 - (*whichbit));
  * curbyte |= tempchar;
  int val = 0;
  if ((*whichbit) == 7)
    {
      int ret;
      ret = fwrite(curbyte, sizeof(unsigned char), 1, fptr);
      //printbyte(*curbyte);
      if(ret == 1)
      {
        val = 1;
      }
      else
      {
        val = -1;
      }
    }
  * whichbit = ((* whichbit) + 1) % 8;
  return val;
}

void charintobits(FILE * outfile, int c, unsigned char * whichbit, unsigned char * curbyte)
{
  unsigned char mask = 0x80;
  while (mask > 0)
    {
      writebit(outfile, (c & mask) == mask, whichbit, curbyte);
      mask >>= 1;
    }
}

int padZero(FILE * fptr, unsigned char *whichbit, unsigned char* curbyte)
{
  int ret = 0; 
  while((*whichbit) != 0)
  {
    ret = writebit(fptr,0,whichbit,curbyte);
  }
  if (ret != -1)
  {
    return -1;
  }
  return ret;
}
//--------------------------------------------------------------------------------------------------------------//
//Header Functions 
//--------------------------------------------------------------------------------------------------------------//
void Tree_headerhelper(tnode * tn, FILE * outfile, unsigned char * whichbit, unsigned char * curbyte)
{
  tnode * leftnode = tn->left;
  tnode * rightnode = tn->right;
  if (tn == NULL)
    {
      return;
    }
    if ((leftnode == NULL) && (rightnode == NULL))
    {
      writebit(outfile, 1, whichbit,curbyte);
      charintobits(outfile, tn->data ,whichbit ,curbyte);
      return;
    }
      Tree_headerhelper(leftnode, outfile, whichbit, curbyte);
      Tree_headerhelper(rightnode, outfile, whichbit, curbyte);
      writebit(outfile, 0, whichbit, curbyte);
  }

void headeroftree(tnode * tn,unsigned int netsize, char * outfile)
{ 
  unsigned char whichbit = 0;
  unsigned char curbyte = 0;
  FILE * fptr = fopen(outfile, "w");
  if (fptr == NULL)
    {
      return;
    }
  //fseek(fptr, 0, SEEK_SET);
  Tree_headerhelper(tn, fptr, &whichbit, &curbyte);
  writebit(fptr, 0, &whichbit, &curbyte);
  padZero(fptr, &whichbit, &curbyte);

  fwrite(&netsize, sizeof(unsigned int),1,fptr);
  printf("%d" ,netsize);
  unsigned char appendlast = '\n';
  fwrite(&appendlast, sizeof(unsigned char),1,fptr);
  fclose (fptr);
}
//------------------------------------------------------------------------------------//
//Helper function , not called for execution but used for debugging 
//------------------------------------------------------------------------------------//
void printbyte(unsigned char bit)
{
  unsigned char mask = 0x80;
  while (mask > 1)
  {
    printf("Current Bit is : %d\n",(bit & mask) == mask);
    mask >>= 1;
  }
}
//---------------------------------------------------------------------------------//
//Table Helper functions 
//---------------------------------------------------------------------------------//
void GeneratetableHelper(tnode* tn , int ** table ,int *row , int col)
{
  if (tn == NULL)
  {
    return;
  }
  tnode * leftchild = tn->left;
  tnode * rightchild = tn->right;

  if ((leftchild == NULL) && (rightchild == NULL))
  {
    table[*row][0] = tn->data;
    (*row)++;
    return;
  }

  if(leftchild != NULL)
  {
    int numRow = tree_leaf(leftchild);
    int k = 0;
    for (k = *row; k < (*row) + numRow; k++)
    {
      table[k][col] = 0;
    }
    GeneratetableHelper(leftchild,table,row,col+1);
  }

    if(rightchild != NULL)
  {
    int numRow = tree_leaf(rightchild);
    int k = 0;
    for (k = *row; k < (*row) + numRow; k++)
    {
      table[k][col] = 1;
    }
    GeneratetableHelper(rightchild,table,row,col+1);
  }
}

void Generatetable(tnode * root , int ** table)
{
  int row = 0;
  GeneratetableHelper(root,table,&row,1);
}
//-----------------------------------------------------------------------------------//
void printtable(int numRow, int **table)
{
//PRINTING TABLE 
  int i =0 ;
  for (i =0; i < numRow; i++)
  {
  	printf("%c ",table[i][0]);
  	int j = 1;
  	while (table[i][j] != -1)
  	{
  		printf("%d ",table[i][j]);
        j++;
    }
    printf("\n");
    }
}

