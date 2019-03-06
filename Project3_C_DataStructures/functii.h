// Nicu Razvan-Alexandru 313CB
/*--- functii.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

typedef struct arbore
{
    void* value;
    struct arbore *left, *right;
}TFrunza, *TArbore;
// struct pentru variabile
typedef struct
{
	char *nume;
	int valoare;
}TVariabila, *AVariabila;
// struct pentru lista de variabile
typedef struct nod
{
	void *info;
	struct nod *urm;
}TCelula, *TLista, **ALista;

typedef int (*TFCmp)(void*, void*); // functie de comparare

// antetele functiilor 

int isOperator(char c);
TArbore creareNod(void *x, size_t d);
void afisareArbore(TArbore arb);
TArbore construieste(char* expression, int* index);
int calculate(int A, int B, void* valoare);
int solve(TArbore t, FILE* f2, TLista listadevariabile, char *variabila, int *sw, TFCmp compara);
void ObtinereSolutie(FILE *f1, FILE *f2);
TLista creareListaVariabile(FILE *f1);
char* creareVectorArbore(FILE *f1, FILE *f2, TLista listadevariabile);
void EliberareLista(ALista lista);
void distrugeArbore(TArbore r);