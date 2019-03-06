// Nicu Razvan-Alexandru 313CB
/*--- main.c ---*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "functii.h"

int main(int argc, char *argv[])
{
	FILE *f1, *f2;
	// deschidere fisiere
	f1 = fopen(argv[1], "at");
		fprintf(f1, "\n");
	fclose(f1);
	f1 = fopen(argv[1], "rt");
	if(!f1)
	{
		fprintf(stderr,"Fisier imposibil de deschis!");
		return -1;
	}
	f2 = fopen(argv[2], "wt");
	if(!f2)
	{
		fprintf(stderr,"Fisier imposibil de creat!");
		return -1;
	}
	ObtinereSolutie(f1,f2);
	fclose(f1);
	fclose(f2);
	return 0;
}
