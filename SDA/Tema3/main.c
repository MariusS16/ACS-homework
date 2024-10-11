// SLINCU MARIUS 315CB
#include "./graph.h"
#include <stdio.h>

int main(int argc, char const* argv[]) {
	FILE* in = fopen("tema3.in", "r");
	FILE* out = fopen("tema3.out", "w");

	int N, M;
	fscanf(in, "%d", &N);
	fscanf(in, "%d", &M);

	Graf* graf = initGraf(N);

	for (int i = 0; i < M; i++) {
		char sursa[30], destinatie[30];
		int distanta;
		fscanf(in, "%s", sursa);
		fscanf(in, "%s", destinatie);
		fscanf(in, "%d", &distanta);
		addEdge(graf, sursa, destinatie, distanta);
		addEdge(graf, destinatie, sursa, distanta);
	}

	int nr = 0;
	int vizitat[graf->n];
	int distante[graf->n];
	for (int i = 0; i < graf->n; i++) {
		vizitat[i] = 0;
		distante[i] = 0;
	}

	for (int i = 0; i < graf->n; i++) {
		if (vizitat[i] == 0) {
			Graf* aux = initGraf(graf->n);
			dfs(graf, i, vizitat, aux);
			distante[nr] = prim(aux);
			nr++;
			free(aux->liste);
			free(aux);
		}
	}

	for (int i = 0; i < nr - 1; i++) { // sortez distantele
		for (int j = i + 1; j < nr; j++) {
			if (distante[i] > distante[j]) {
				int aux = distante[i];
				distante[i] = distante[j];
				distante[j] = aux;
			}
		}
	}

	fprintf(out, "%d\n", nr);
	for (int i = 0; i < nr; i++) {
		fprintf(out, "%d\n", distante[i]);
	}

	for (int i = 0; i < graf->n; i++) {
		Nod* aux = graf->liste[i]->head;
		while (aux != NULL) {
			Nod* aux2 = aux->next;
			free(aux);
			aux = aux2;
		}
		free(graf->liste[i]);
	}
	free(graf->liste);
	free(graf);

	fclose(in);
	fclose(out);
	return 0;
}