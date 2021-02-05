#ifndef HUFF
#define HUFF
#include<stdio.h>

typedef struct
{
  unsigned char val;
  int weight;
}Freq;

typedef struct tnode
{
  struct tnode * left;
  struct tnode * right;
  unsigned char data;
  int weight;
}tnode;

typedef struct lnode
{
  struct lnode* next;
  tnode* ptr;
}lnode;

//------------------------------------------------------//
//Encode functions 
int count(char* infile, Freq* frequencies);
int compare(const void* p1, const void* p2);
void sort(Freq* frequencies);
//---------------------------------------------------------//
lnode* Listbuild(Freq* frequencies);
lnode* InsertintoList(lnode *head, lnode* listcurrnode);
lnode* CreatesListNode(tnode* treenode);
void Headcheck(lnode* head);
tnode* CleanList(lnode *head);
//-----------------------------------------------------//
tnode* CreatesTree(unsigned char val, int freqvalue);
tnode* MergesTree(tnode* tn1, tnode* tn2);
void PrefixTreePrint(tnode* root , int level);
void destroytree(tnode* root);
//----------------------------------------------------//
lnode* Huffman(lnode* head);
int encode(char* infile, char* outfile);
//----------------------------------------------------//
int tree_height (tnode* tn);
int tree_leaf(tnode * tn);
//----------------------------------------------------------------------//
void Generatetable(tnode* root , int** table);
void GeneratetableHelper(tnode* tn , int** table ,int* row , int col);
void printtable(int numRow, int** table);
//----------------------------------------------------------------------------------------------------------------//
void Tree_headerhelper(tnode* tn, FILE* outfile, unsigned char* whichbit,unsigned char* curbyte);
void headeroftree(tnode* tn,unsigned int netszie, char* outfile);
int writebit(FILE* fptr, unsigned char bit, unsigned char* whichbit, unsigned char* curbyte);
void printbyte(unsigned char bit);
int padZero(FILE* fptr, unsigned char* whichbit, unsigned char* curbyte);
void charintobits(FILE* outfile,int c, unsigned char* whichbit, unsigned char* curbyte);
int compressionbitmanipulation(char* infile, char* outfile, int** table, int* map);
//----------------------------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------------------------//
//Decode functions 
int unhuff(char* infile, char* outfile);
tnode* read_header(FILE* infile);
int readBit(FILE* fptr, unsigned char* bit, unsigned char* whichbit, unsigned char* curbyte);
//----------------------------------------------------------------------------------------------------------------//
#endif