//=========================================================
// Created by: Abhimanyu Agarwal
// PUID : 0031135872
// Project - 1 ECE 368
//=========================================================

//Header Files
#include <stdio.h>
#include "sorting.h"


//Sequence1
int* Seq1(int N, int *Size){
    int *res = malloc(sizeof(int)*N);
    res[0] = 1;
    int i;
    int p2 = 0;
    int p3 = 0;
    i = 1;
    	while (res[i-1] < N)
    	{
        	if (res[p2] * 2 == res[i - 1]){
            	p2 += 1;
        	}
        	if (res[p3] * 3 == res[i - 1]){
            	p3 += 1;
        	}
        	int u2 = res[p2] * 2;
        	int u3 = res[p3] * 3;
        	if (u2 < u3){
            	p2 += 1;
            	res[i] = u2;
        	}
        	else{
            	p3 += 1;
            	res[i] = u3;
        	}
        	i++;
        }
    *Size = i - 1;
return res;
}



void Shell_Insertion_Sort (long *Array, int Size, double *NComp, double *NMove)
{
	int gap1; 
	int *gapvalue1 = Seq1(Size,&gap1);
	int i;
	int j;
	int k;
	(*NMove) = 0;
	(*NComp) = 0;
	for (i = gap1 - 1; i >= 0;i--)
	{
		int current1 = gapvalue1[i];
		for (j = current1; j < Size; j++)
		{
			long val1 = Array[j];
			k = j;
			while ((k >= current1) && Array[k - current1] > val1)
			{
	        	Array[k] = Array[k - current1];	
				(*NComp)++;
				(*NMove)++;
				k = k - current1;
			}
			(*NComp)++;
			Array[k] = val1;
			(*NMove)++;
		}
	}
	free(gapvalue1);
}

int *Seq2(int N, int* Size)
{
	int *res2 = malloc(sizeof(int)*N);
	int i = 0;
	float value = 1.3;   //Divide by 1.3 
	while( N != 1)
	{ 
		N = (N / value);
		res2[i] = N;
	if (res2[i] == 9 || res2[i] == 10)
		{
			res2[i] = 11;
		}
		i++;
	}
*Size = i;
return res2;
}

void Improved_Bubble_Sort (long *Array, int Size, double *NComp, double *NMove)
{
	int gap2; 
	int *gapvalue2 = Seq2(Size,&gap2);
	int i;
	int j;
	int k;                
	(*NComp) = 0;
	(*NMove) = 0;
	for (i = 0; i < gap2 ; i++)
	{
		int current2 = gapvalue2[i]; 
		for(j = current2; j < Size; j++) 
		{
			k = j;
			while((k >= current2) && (Array[k - current2] > Array[k]))
				{
				(*NComp)++;
				long val2 = Array[k];
				Array[k] = Array[k - current2];
				Array[k - current2] = val2;
				(*NMove) += 3;
				k = k - current2;
		    	}
		    (*NComp)++;
		}
	}
	free(gapvalue2);
}

//Load file function 
long *Load_File(char *Filename, int *Size)
{
	long i;
	FILE *fptr = fopen(Filename, "rb");
	if (fptr == NULL)
	{
		printf("File is NULL\n");
	}
	fscanf(fptr, "%d\n", Size);
	long * Array = malloc (sizeof(long)* *Size);
	for (i = 0; i < *Size; i++)
	{
		fscanf(fptr, "%ld\n", Array+i);
	}
	fclose(fptr);
	return Array;
}

//Save file function 
int Save_File(char *Filename, long *Array, int Size)
{
	int i = 0;
	FILE *fptr = fopen(Filename, "wb");
	if (fptr == NULL)
	{
		printf("File is NULL\n");
	}
	fprintf(fptr, "%d\n", Size);	
	for(i = 0; i < Size; i++)
	{
		fprintf(fptr, "%ld\n", Array[i]);
	}
	fclose(fptr);
	return Size;
}

//Saves sequence1
void Save_Seq1 (char *Filename, int N)
{
	int Seq1_Size;
	int i = 0; 	
	int * Seq_1 = Seq1(N, &Seq1_Size);
	FILE *outfile1 = fopen(Filename, "wb");
	if (outfile1 == NULL)
	{
	printf("File is NULL\n");
	return; 
	}	
	fprintf(outfile1, "%d", Seq1_Size);
	for (i = 0; i < Seq1_Size; i++)
	{
	fprintf(outfile1, "%d", Seq_1[i]);
	}
	free(Seq_1);
}

void Save_Seq2 (char *Filename, int N)
{
	int Seq2_Size;
	int i = 0; 	
	int * Seq_2 = Seq2(N, &Seq2_Size);
	FILE *outfile2 = fopen(Filename, "wb");
	if (outfile2 == NULL)
	{
	printf("File is NULL\n");
	return;
	}
	fprintf(outfile2, "%d", Seq2_Size);	
	for (i = Seq2_Size - 1; i >=0 ; i--)
	{
		fprintf(outfile2, "%d", Seq_2[i]);
	}
	free(Seq_2);
}

