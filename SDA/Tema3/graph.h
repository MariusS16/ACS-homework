// SLINCU MARIUS 315CB
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
	char nume[30];
	int distanta;
	struct nod* next;
} Nod;

typedef struct muchie {
	char sursa[30], destinatie[30];
	int distanta;
} Muchie;

typedef struct lista {
	Nod* head;
} Lista;

typedef struct graf {
	int n;
	Lista** liste;
} Graf;

Graf* initGraf(int n);
int getIndex(Graf* graf, char nume[30]);
void createList(Graf* graf, char nume[30]);
void addEdge(Graf* graf, char sursa[30], char destinatie[30], int distanta);
void dfs(Graf* graf, int index, int vizitat[], Graf* aux);
int prim(Graf* graf);

#endif