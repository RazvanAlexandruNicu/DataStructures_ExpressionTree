// Nicu Razvan-Alexandru 313CB
#include "functii.h"
/*
 * Functie ce realizeaza alocarea
 * unei celule (TLista) pentru lista
 * de variabile.
*/

TLista AlocaCelula(void *x)
{
	TLista aux = (TLista)malloc(sizeof(TCelula));
	if(!aux)
	{
		return NULL;
	}
	aux -> info = x;
	aux -> urm = NULL;
	return aux;
}

/*
 * Functie ce realizeaza compararea
 * a doua stringuri. Prime 2 void*
 * pe care ii converteste la tipul de data
 * dorit (numele unei variabile si informatia din
 * nodul respectiv din arbore)
*/

int compara(void* x, void* y)
{
	return strcmp(((AVariabila)(((TLista)x) -> info)) -> nume,
				  (char*)(((TArbore)y) -> value));
}

/*
 * Functie ce realizeaza inserarea
 * unui element de tip celula la inceputul
 * listei simplu inlantuire te variabile
*/

void inserareInceput(ALista L, TLista celula)
{
	celula -> urm = *L;
	*L = celula;
	// am inserat la inceputul listei.
}

/*
 * Functie ce returneaza 1 daca c este
 * un operator cunoscut si 0 in rest
*/

int isOperator(char c)
{
    if (c == '+' || c == '-' ||
            c == '*' || c == '/' ||
            c == '^' || c == '#')
        return 1;
    return 0;
}

/*
 * Functie creeaza un nod pentru arbore
 * in care pune informatia x cu dimensiunea d
*/

TArbore creareNod(void *x, size_t d)
{
	TArbore nod = (TArbore)malloc(sizeof(TFrunza));
	if(!nod)
	{
		return NULL;
	}
	nod -> value = malloc(d);
	if(!nod -> value)
	{
		free(nod);
		return NULL;
	}
	memcpy(nod -> value, x, d);
	nod -> left = NULL;
	nod -> right = NULL;
	return nod;
}
 
/*
 * Functie ce primeste ca parametru 2 intregi
 * si operatia dorita si realizeaza operatia
 * returnand valoarea sau codul de eroare in cazul
 * in care este nevoie
*/

int calculate(int A, int B, void* valoare)
{
	if(((char*)(valoare))[0] == '+')
	{	
		return A + B;
	}
	if(((char*)(valoare))[0] == '-')
	{
		return A - B;
	}
	if(((char*)(valoare))[0] == '*')
	{
		return A * B;
	}
	if(((char*)(valoare))[0] == '/')
	{
		if(B == 0)
		{
			return -999; // impartire la 0
		}
		return A / B;
	}
	if(((char*)(valoare))[0] == '^')
	{
		return pow(A, B);
	}
	if(((char*)(valoare))[0] == '#')
	{
		if(A < 0) // avem numar negativ
		{
			return -999; // radacina din nr negativ
		}
		return sqrt(A);
	}	
}

/*
 * Functie ce realizeaza afisarea in consola
 * a arborelui dat ca parametru
 * Am folosit functia pentru debugging si am zis
 * sa o las aici, chiar daca in varianta finala a
 * codului nu o voi apela.
*/

void afisareArbore(TArbore arb)
{
	if(arb == NULL)
	{
		return;
	}
	if(isOperator(*(char*)(arb -> value)))
	{
		printf("Operator:%c\n", *(char*)(arb -> value));
	}
	else
	{
		printf("%s\n", (char*)(arb -> value));
	}
	afisareArbore(arb -> left);
	afisareArbore(arb -> right);
}

/*
 * Functie ce realizeaza construirea arborelui
 * plecand de la o expresie (vector de caractere)
 * dat ca parametru
*/

TArbore construieste(char* expression, int* index)
{
	TArbore nodnou = NULL;
	(*index)++;
	char *p = NULL;
	int i;
	char* copieexpresie = strdup(expression);
	// fac o copie expresiei inainte de a incepe
	p = strtok(copieexpresie, " ");
	if(*index == 1)
	{
		p = strtok(NULL, " ");
	}
	if(*index >= 2)
	{
		for(i = 0; i < *index ; i++)
		{
			p = strtok(NULL, " ");
			if(p == NULL)
			{
				break;
			}
		}
	}
	if(p != NULL)
	{
		// am strtok pe al 'index'-lea -1 cuvant
		if(isOperator(*p)) // daca p este operator.
		{
			nodnou = creareNod((void*)p, strlen(p) * sizeof(char) + 1);
			if(!nodnou)
			{
				return NULL;
			}
			if(*p != '#') // daca e operator binar
			{
				nodnou -> left = construieste(expression, index);
				nodnou -> right = construieste(expression, index);
			}
			else // daca e operator unar
			{
				nodnou -> left = construieste(expression, index);
			}

		} else // daca nu e operator, creez nodul cu variabila.
		{
			nodnou = creareNod((void*)p, strlen(p)*sizeof(char) + 1);
			if(!nodnou)
			{
				return NULL;
			}
		}
	}
	free(copieexpresie); // eliberez variabila cu copia expresiei
	return nodnou;
}

 /*
 * Functie ce rezolva ecuatia din arborele t
 * Primeste listadevariabile, un string care
 * returneaza prin efect lateral numele variabilei
 * Sw - variabila ce retine -1, 1 in cazul unei erori
 */

int solve(TArbore t, FILE* f2, TLista listadevariabile,
		  char *variabila, int *sw, TFCmp compara)
{
	int A, B;
	int valoare = 0;
	char primulcaracter; 
	// daca nu avem pana acum vreo eroare
	if(*sw == 0)
	{
		if(t != NULL)
		{
			TLista iter = NULL;
			// daca este un numar
			primulcaracter = ((char*)(t -> value))[0];
			if( primulcaracter >= '0' && primulcaracter <= '9') 
			{
				return atoi((char*)(t->value));
			}
			// daca este o variabila
			if(!isOperator(*(char*)(t->value)))
			{
				// verific existenta in lista de variabile
				for(iter = listadevariabile; iter != NULL; iter = iter -> urm)
				{
					if(compara(iter, t)==0)
					{
						valoare = ((AVariabila)(iter -> info)) -> valoare;
						break;
					}
				}
				// daca nu exista variabila
				if(iter == NULL) 
				{
					strcpy(variabila, (char*)t -> value);
					// cod de eroare in cazul in care nu avem variabila
					*sw = 1;
					return 666;
				}
				// returnez valoarea variabilei respective.
				return valoare; 
			}
			else // daca e operator
			{
				A = solve(t -> left, f2, listadevariabile, variabila, sw, compara);
				B = solve(t -> right, f2, listadevariabile, variabila, sw, compara);
				if(A == 666 || B == 666)
				{
					*sw = 1;
					return 666;
				}
				if(A == -999 || B == -999)
				{
					*sw = -1;
					return -999;
				}
				if(calculate(A, B, t -> value) == -999)
				{
					*sw = -1;
					return -999;
				}
				// daca avem coduri de eroare returnam codul
				// si schimbam valoarea variabilei sw
				return calculate(A, B, t -> value);
			}
		}	
	}
}

/*
 * Functie ce realizeaza afisarea listei de variabile
 * Functia este facuta pentru debugging si am hotarat
 * sa o las totusi aici, chiar daca in varianta finala
 * a codului nu o mai folosesc.
*/

void afisareLista(TLista L)
{
	TLista p = L;
	AVariabila variabila = (AVariabila)(p -> info);
	while(p)
	{
		printf("%s %d\n", variabila -> nume, variabila -> valoare);
		p = p -> urm;
	}
}

/*
 * Functie ce realizeaza crearea listei de variabile
 * citind de la tastatura numarul de variabile
 * si cele N asignari de variabile
 * Insereaza cu ajutorul functiilor anterioare variabila
 * la inceputul listei de variabile
*/

TLista creareListaVariabile(FILE *f1)
{
	int N1, i;
	fscanf(f1, "%d\n", &N1);
	char *buffer, *numevariabila, *p;
	buffer  = (char*)calloc(300, sizeof(char));
	// buffer in care citesc liniile din fisierul de input
	TLista celulanoua = NULL;
	TLista listadevariabile = NULL;
	if(!buffer)
	{
		printf("Alocare esuata!\n");
		return NULL;
	}
	for (i = 0; i < N1; i++)
	{
		fgets(buffer, 300, f1);
		buffer[strlen(buffer) - 1] = '\0';
		AVariabila varnoua = (AVariabila)malloc(sizeof(TVariabila));
		if(!varnoua)
		{
			printf("Alocare esuata!\n");
			return NULL;
		}
		// pun numele in structul variabila
		p = strtok(buffer, " ");
		varnoua -> nume = strdup(p);
		// in p se afla numele variabilei
		p = strtok(NULL, " ");
		// in p se afla '='
		p = strtok(NULL, " ");
		// in p se afla valoarea variabilei.
		varnoua -> valoare = strtol(p, NULL, 10);
		celulanoua = AlocaCelula(varnoua);
		if(!celulanoua)
		{
			return NULL;
		}
		inserareInceput(&listadevariabile, celulanoua);
	}
	free(buffer);
	return listadevariabile;
}

/*
 * Functie ce realizeaza crearea vectorului folosit
 * in crearea arborelui. Primeste expresia data in input
 * si reduce sqrt la '#' pentru o evaluare mai usoara
 * , punand de altfel si variabilele si ceilalti operatori
*/

char* creareVectorArbore(FILE *f1, FILE *f2, TLista listadevariabile)
{
	TLista iterator = NULL;
	char *buffer = NULL, *vectornou = NULL, *p = NULL;
	buffer  = (char*)calloc(300, sizeof(char));
	vectornou  = (char*)calloc(300, sizeof(char));
	fgets(buffer, 300, f1);
	buffer[strlen(buffer) - 1] = '\0';
	char locpentruvaloare[12];
	// in buffer am forma prefix pentru definirea arborelui
	p = strtok(buffer, " ");
	while(p != NULL)
	{
		if(strcmp(p, "+")==0 || strcmp(p, "-")==0  || strcmp(p, "*")==0 ||
		   strcmp(p, "/")==0 || strcmp(p, "^")==0 || strcmp(p, "sqrt")==0)
		{
			if(strcmp(p, "sqrt")==0)
			{
				// pun # in loc de sqrt pentru o mai buna recunoastere a operatiei.
				strcpy(p, "#"); 
			}
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, " "); // concatenez
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, p); // concatenez
		}
		else if(p[0] >= 'a' && p[0] <= 'z') // daca e variabila 
		{
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, " "); // concatenez
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, p); // concatenez
		}
		else // daca e numar
		{
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, " "); // concatenez
			vectornou[strlen(vectornou)] = '\0'; // pun terminator
			vectornou = strcat(vectornou, p); // concatenez
		}
		p = strtok(NULL, " ");
	}
	free(buffer);
	return vectornou;
}

/*
 * Functie ce realizeaza eliberarea listei de variabile
 * parcurgand-o element cu element si eliberand
 * continutul acestora.
*/

void EliberareLista(ALista lista)
{
	TLista p = NULL, temp = NULL;
	for(p = *lista; p != NULL;)
	{
		temp = p;
		p = p -> urm;
		free(((AVariabila)(temp -> info)) ->nume);
		free(temp -> info);
		free(temp);
	}
	*lista = NULL;
	// eliberez lista de variabile
}

/*
 * Functie ce realizeaza distrugerea arborelui
 * prin apel recursiv, eliberand valoarea alocata
 * si nodul actual.
*/

void distrugeArbore(TArbore r) 	/* functie auxiliara - distruge toate nodurile */
{
	if (!r)
	{
		return;
	}
	distrugeArbore (r -> left);     /* distruge subarborele stang */
	distrugeArbore (r -> right);     /* distruge subarborele drept */ 
	free(r -> value);
	free(r);
}

/*
 * Functie principala in care se apeleaza functiile anterioare.
 * Se citesc cele n expresii , se evalueaza si se returneaza
 * apelul functiei solve.
*/

void ObtinereSolutie(FILE *f1, FILE *f2)
{

	int N2, i, sw = 0;
	TLista listadevariabile = creareListaVariabile(f1);
	//afisareLista(listadevariabile);
	char variabila[10];
	fscanf(f1, "%d\n", &N2);
	TArbore tree = NULL;
	for(i = 0; i < N2; i++)
	{
		sw = 0;
		char *vectornou = NULL;
		vectornou = creareVectorArbore(f1, f2, listadevariabile);
		// fac un vector in care pun intructiunea simplificata '#' in loc de sqrt.
		int index = -1;
	    tree = construieste(vectornou, &index);
	   	solve(tree, f2, listadevariabile, variabila, &sw, compara);
		if(sw == -1)
		{
			fprintf(f2, "Invalid expression\n");
		}
		else if(sw == 1)
		{
			fprintf(f2, "Variable %s undeclared\n", variabila);
		}
		else if(solve(tree, f2, listadevariabile, variabila, &sw, compara) < -9999999)
		{
			fprintf(f2, "0\n");
		}
		else
		{
			fprintf(f2, "%d\n", solve(tree, f2, listadevariabile, variabila, &sw, compara));
		}
		// distrug arborele
		distrugeArbore(tree);
	    free(vectornou);
	}
	// eliberez lista de variabila
	EliberareLista(&listadevariabile);
}

	
