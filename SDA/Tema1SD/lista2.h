#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TLista {
	char *info;
	struct TLista *urm;
};

struct TLista2 {
	char info;
	struct TLista2 *pre, *urm;
};

struct TListaNod {
	// retine pozitia degetului
	struct TLista2 *info;
	struct TListaNod *urm;
};

struct TBanda {
	struct TLista2 *deget;
	struct TLista2 *santinela;
};

struct TCoada {
	struct TLista *inc, *sf;
};

struct TStiva {
	struct TListaNod *inc;
};

struct TLista2 *AlocCelula2(char x);
struct TLista2 *InitLista2();

void SHOW(struct TBanda *B, FILE *out);
void SHOW_CURRENT(struct TBanda *B, FILE *out);
void MOVE_LEFT(struct TBanda **B);
void MOVE_RIGHT(struct TBanda **B);
void MOVE_LEFT_CHAR(struct TBanda **B, char c, FILE *out);
void MOVE_RIGHT_CHAR(struct TBanda **B, char c);
void WRITE(struct TBanda **B, char c);
void INSERT_LEFT(struct TBanda **B, char c, FILE *out);
void INSERT_RIGHT(struct TBanda **B, char c);

struct TCoada *InitQ();
int ExtrQ(struct TCoada **c, char *x);
int IntrQ(struct TCoada **c, char *x);
void DistrQ(struct TCoada **c);
void AfisareQ(struct TCoada *c);
struct TStiva *InitS();
void IntrS(struct TStiva **s, struct TLista2 *nod);
void ExtrS(struct TStiva **s);

void EXECUTE(struct TCoada **c, struct TBanda **B, struct TStiva **S, FILE *out);