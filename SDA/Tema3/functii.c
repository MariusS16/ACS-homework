// SLINCU MARIUS 315CB
#include "./graph.h"

Graf* initGraf(int n) {
	Graf* graf = (Graf*)malloc(sizeof(Graf));
	graf->n = 0;
	graf->liste = (Lista**)malloc(n * sizeof(Lista*));
	return graf;
}

int getIndex(Graf* graf, char nume[30]) {
	for (int i = 0; i < graf->n; i++) {
		if (strcmp(graf->liste[i]->head->nume, nume) == 0) {
			return i;
		}
	}
	return -1;
}

void createList(Graf* graf, char nume[30]) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	strcpy(nod->nume, nume);
	nod->distanta = 0;
	nod->next = NULL;
	graf->liste[graf->n] = (Lista*)malloc(sizeof(Lista));
	graf->liste[graf->n]->head = nod;
	graf->n++;
}

void addEdge(Graf* graf, char sursa[30], char destinatie[30], int distanta) {
	int sursaIndex = getIndex(graf, sursa);
	if (sursaIndex == -1) {
		createList(graf, sursa);
		sursaIndex = getIndex(graf, sursa);
	}
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	strcpy(nod->nume, destinatie);
	nod->distanta = distanta;
	Nod* aux = graf->liste[sursaIndex]->head;
	while (aux->next != NULL) {
		aux = aux->next;
	}
	aux->next = nod;
}

void dfs(Graf* graf, int index, int vizitat[], Graf* comp) {
	vizitat[index] = 1;
	Nod* aux = graf->liste[index]->head;
	aux = aux->next;
	while (aux != NULL) {
		int idx = getIndex(graf, aux->nume);
		if (vizitat[idx] == 0) {
			dfs(graf, idx, vizitat, comp);
		}
		aux = aux->next;
	}
	comp->liste[comp->n] = graf->liste[index];
	comp->n++;
}

int prim(Graf* graf) {
	int distante[graf->n];
	int vizitat[graf->n];
	for (int i = 0; i < graf->n; i++) {
		distante[i] = 999999;
		vizitat[i] = 0;
	}
	distante[0] = 0;
	for (int i = 0; i < graf->n; i++) {
		int min = 999999;
		int index = -1;
		for (int j = 0; j < graf->n; j++) {
			if (vizitat[j] == 0 && distante[j] < min) {
				min = distante[j];
				index = j;
			}
		}
		vizitat[index] = 1;
		Nod* aux = graf->liste[index]->head;
		aux = aux->next;
		while (aux != NULL) {
			int idx = getIndex(graf, aux->nume);
			if (vizitat[idx] == 0 && aux->distanta < distante[idx]) {
				distante[idx] = aux->distanta;
			}
			aux = aux->next;
		}
	}
	int suma = 0;
	for (int i = 0; i < graf->n; i++) {
		suma += distante[i];
	}
	return suma;
}