#include "lista2.h"

void MOVE_LEFT(struct TBanda **B) {
	if ((*B)->deget->pre->info != (*B)->santinela->info)
		(*B)->deget = (*B)->deget->pre;
}

void MOVE_RIGHT(struct TBanda **B) {
	if ((*B)->deget->urm == NULL) {
		struct TLista2 *aux = AlocCelula2('#');
		struct TLista2 *p = (*B)->santinela;

		while ((*B)->santinela->urm)
			(*B)->santinela = (*B)->santinela->urm;

		(*B)->santinela->urm = aux;
		aux->pre = (*B)->santinela;

		(*B)->santinela = p;
		(*B)->deget = (*B)->deget->urm;
	} else {
		(*B)->deget = (*B)->deget->urm;
	}
}

void MOVE_LEFT_CHAR(struct TBanda **B, char c, FILE *out) {
	struct TLista2 *aux = (*B)->deget;
	int ok = 1;
	while ((*B)->deget->info != c) {
		if ((*B)->deget->pre == NULL) {
			fprintf(out, "ERROR\n");
			ok = 0;
			break;
		}
		(*B)->deget = (*B)->deget->pre;
	}
	if (ok == 0)
		(*B)->deget = aux;
}

void MOVE_RIGHT_CHAR(struct TBanda **B, char c) {
	while ((*B)->deget->info != c) {
		if ((*B)->deget->urm == NULL) {
			struct TLista2 *aux = AlocCelula2('#');
			struct TLista2 *p = (*B)->santinela;
			while ((*B)->santinela->urm)
				(*B)->santinela = (*B)->santinela->urm;
			aux->pre = (*B)->santinela->urm;
			(*B)->santinela->urm = aux;

			(*B)->santinela = p;
			(*B)->deget = (*B)->deget->urm;
			break;
		}
		(*B)->deget = (*B)->deget->urm;
	}
}

void WRITE(struct TBanda **B, char c) {
	(*B)->deget->info = c;
}

void INSERT_LEFT(struct TBanda **B, char c, FILE *out) {
	if ((*B)->deget->pre == (*B)->santinela) {
		fprintf(out, "ERROR\n");
	} else {
		struct TLista2 *aux = AlocCelula2(c);
		aux->urm = (*B)->deget;
		aux->pre = (*B)->deget->pre;
		(*B)->deget->pre->urm = aux;
		aux->urm->pre = aux;
		(*B)->deget = (*B)->deget->pre;
	}
}

void INSERT_RIGHT(struct TBanda **B, char c) {
	struct TLista2 *aux = AlocCelula2(c);
	if ((*B)->deget->urm) {
		aux->urm = (*B)->deget->urm;
		aux->pre = (*B)->deget;
		(*B)->deget->urm = aux;
		aux->urm->pre = aux;
		(*B)->deget = (*B)->deget->urm;
	} else {
		aux->pre = (*B)->deget;
		(*B)->deget->urm = aux;
		(*B)->deget = (*B)->deget->urm;
	}
}

void SHOW_CURRENT(struct TBanda *B, FILE *out) {
	fprintf(out, "%c\n", B->deget->info);
}

void SHOW(struct TBanda *B, FILE *out) {
	struct TLista2 *aux = B->santinela->urm;

	while (aux != NULL) {
		if (aux == B->deget) {
			fprintf(out, "|%c|", aux->info);
		} else {
			fprintf(out, "%c", aux->info);
		}
		aux = aux->urm;
	}
	fprintf(out, "\n");
}

struct TLista2 *AlocCelula2(char x) {
	struct TLista2 *aux = calloc(1, sizeof(struct TLista2));
	if (!aux) {
		return NULL;
	}

	aux->info = x;
	aux->pre = aux->urm = NULL;

	return aux;
}

/* Creeaza santinela pentru lista folosita */
struct TLista2 *InitLista2() {
	struct TLista2 *aux = calloc(1, sizeof(struct TLista2));
	if (!aux) {
		return NULL;
	}

	aux->pre = NULL;
	aux->urm = NULL;

	struct TLista2 *node = AlocCelula2('#');
	aux->urm = node;
	node->pre = aux;

	return aux;
}

/* Distrugere lista */
// void DistrugeLista2(struct TLista2 *aL) {
//     struct TLista2 *p = (*aL)->urm, aux;
//     while (p != NULL) {         /* distrugere elementele listei */
//         aux = p;
//         p = p->urm;
//         free(aux);
//     }

//     free(*aL);                  /* distrugere santinela */
//     *aL = NULL;
// }

struct TCoada *InitQ() /* creeaza coada vida cu elemente de dimensiune d;
					anumite implementari pot necesita si alti parametri */
{
	struct TCoada *c; /* spatiu pentru descriptor coada */
	c = calloc(1, sizeof(struct TCoada));
	if (!c) return NULL; /* nu exista spatiu -> "esec" */

	c->inc = c->sf = NULL;
	return c; /* intoarce adresa descriptorului cozii */
}

int IntrQ(struct TCoada **c, char *operation) /* adauga element la sfarsitul cozii */
{
	struct TLista *aux;
	aux = calloc(1, sizeof(struct TLista)); /* aloca o noua celula */
	if (!aux) return 0;											/* alocare imposibila -> "esec" */

	aux->info = calloc(1, sizeof(strlen(operation) + 1));
	strcpy(aux->info, operation);
	aux->urm = NULL;

	if ((*c)->sf != NULL)	 /* coada nevida */
		(*c)->sf->urm = aux; /* -> leaga celula dupa ultima din coada */
	else									 /* coada vida */
		(*c)->inc = aux;		 /* -> noua celula se afla la inceputul cozii */
	(*c)->sf = aux;				 /* actualizeaza sfarsitul cozii */
	return 1;							 /* operatie reusita -> "succes" */
}

int ExtrQ(struct TCoada **c, char *x) /* extrage primul element din coada la adresa ae */
{
	struct TLista *aux = NULL;
	if ((*c)->inc == NULL)
		return 0;
	aux = (*c)->inc;
	strcpy(x, aux->info);
	if ((*c)->inc == (*c)->sf) {
		(*c)->inc = NULL;
		(*c)->sf = NULL;
	} else {
		(*c)->inc = (*c)->inc->urm;
	}
	free(aux);
	return 1;
}

void AfisareQ(struct TCoada *c) /* afisare elementele cozii */
{
	struct TLista *p;
	if (c->inc == NULL) /* coada vida */
	{
		printf("Coada vida\n");
		return;
	}
	for (p = c->inc; p != NULL; p = p->urm)
		printf("%s\n", p->info);
	printf("\n");
}

void DistrQ(struct TCoada **c) /* distruge coada */
{
	struct TLista *p, *aux;
	p = (*c)->inc;
	while (p) {
		aux = p;
		p = p->urm;
		free(aux);
	}
	free(*c);
	*c = NULL;
}

struct TStiva *InitS() {
	struct TStiva *s; /* spatiu pentru descriptor stiva */
	s = calloc(1, sizeof(struct TStiva));
	if (!s) return NULL; /* nu exista spatiu -> "esec" */

	s->inc = NULL;
	return s; /* intoarce adresa descriptorului cozii */
}

void IntrS(struct TStiva **s, struct TLista2 *nod) /* adauga element in stiva */
{
	struct TListaNod *aux = calloc(1, sizeof(struct TListaNod));
	aux->info = nod;
	aux->urm = (*s)->inc;
	(*s)->inc = aux;
}

void ExtrS(struct TStiva **s) /* extrage ultimul element din stiva */
{
	struct TListaNod *aux = (*s)->inc;
	(*s)->inc = (*s)->inc->urm;
	free(aux);
}

void EXECUTE(struct TCoada **c, struct TBanda **B, struct TStiva **S, FILE *out) {
	char command[30], op[30], chr;
	ExtrQ(c, command);
	sscanf(command, "%s %c", op, &chr);
	if (strcmp("MOVE_LEFT", op) == 0) {
		if ((*B)->deget->pre != (*B)->santinela) {
			IntrS(S, (*B)->deget);	// baga in stiva doar daca se executa operatia
			(*B)->deget = (*B)->deget->pre;
		}
	}
	if (strcmp("MOVE_RIGHT", op) == 0) {
		IntrS(S, (*B)->deget);
		MOVE_RIGHT(B);
	}
	if (strcmp("MOVE_LEFT_CHAR", op) == 0)
		MOVE_LEFT_CHAR(B, chr, out);
	if (strcmp("MOVE_RIGHT_CHAR", op) == 0)
		MOVE_RIGHT_CHAR(B, chr);
	if (strcmp("WRITE", op) == 0)
		WRITE(B, chr);
	if (strcmp("INSERT_LEFT", op) == 0)
		INSERT_LEFT(B, chr, out);
	if (strcmp("INSERT_RIGHT", op) == 0)
		INSERT_RIGHT(B, chr);
}