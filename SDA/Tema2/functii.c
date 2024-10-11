// SLINCU MARIUS 315CB
#include "arb.h"

unsigned long long CalcMean(Color **matrice, int x, int y, unsigned int dim, Color *culori) {
    unsigned long long red = 0, green = 0, blue = 0;
    for (int i = x; i < x + dim; i++)
        for (int j = y; j < y + dim; j++) {
            red+= matrice[i][j].rs;
            green+= matrice[i][j].vr;
            blue+= matrice[i][j].ab;
        }
    red = red / (dim * dim);// calculam conform formulei pentru red
    green = green / (dim * dim);// calculam conform formulei pentru green
    blue = blue / (dim * dim);// calculm conform formulei pentru blue

    unsigned long long mean = 0;
    for (int i = x; i < x + dim; i++)
        for (int j = y; j < y + dim; j++) {
            mean+= (red - matrice[i][j].rs)*(red - matrice[i][j].rs);
            mean+= (green - matrice[i][j].vr)*(green - matrice[i][j].vr);
            mean+= (blue - matrice[i][j].ab)*(blue - matrice[i][j].ab);
        }
    mean = mean / (3 * dim * dim); // calculam conform formulei mean

    culori->rs = red;
    culori->vr = green;
    culori->ab = blue; //retinem fiecare rezultat obtinut dupa calcularea formulei

    return mean;
}

TArb arbore(Color **matrice, int x, int y, unsigned int dim, int factor){
    TArb arb = calloc(1, sizeof(TNod));
    Color culori;
    unsigned long long mean = CalcMean(matrice, x, y, dim, &culori);
    if (mean > factor) {
        arb->tip = 0;
        arb->Ss = arbore(matrice, x, y, dim / 2, factor);
        arb->Ds = arbore(matrice, x, y + dim / 2, dim / 2, factor);
        arb->Dj = arbore(matrice, x + dim / 2, y + dim / 2, dim / 2, factor);
        arb->Sj = arbore(matrice, x + dim / 2, y, dim / 2, factor);
    } else {
        arb->tip = 1;
        arb->info.rs = culori.rs;
        arb->info.vr = culori.vr;
        arb->info.ab = culori.ab;
        arb->Ss = NULL;
        arb->Ds = NULL;
        arb->Dj = NULL;
        arb->Sj = NULL;
    }

    return arb;
}

void dezalocaArbore(TArb r) {
    if (r == NULL) return;

    if (r->Ss) dezalocaArbore(r->Ss);
    if (r->Ds) dezalocaArbore(r->Ds);
    if (r->Dj) dezalocaArbore(r->Dj);
    if (r->Sj) dezalocaArbore(r->Sj);
    free(r);
}

int NrNiv(TArb r)           /* numar niveluri (0 pentru arbore vid) */
{
	int Ss, Ds, Dj, Sj;

	if (r == NULL) return 0;

    int max = 0;
	Ss = NrNiv(r->Ss);
    if (max < Ss) {
        max = Ss;
    }

    Ds = NrNiv(r->Ds);
    if (max < Ds) {
        max = Ds;
    }

    Dj = NrNiv(r->Dj);
    if (max < Dj) {
        max = Dj;
    }

    Sj = NrNiv(r->Sj);
    if (max < Sj) {
        max = Sj;
    }

	return max + 1;
}

int frunze(TArb r) {
	if (r == NULL)
		return 0;
	if (r->Ss == NULL && r->Ds == NULL && r->Dj == NULL && r->Sj == NULL)
		return 1;
	return frunze(r->Ss) + frunze(r->Ds) + frunze(r->Dj) + frunze(r->Sj);
}

int niv_frunze(TArb r){
	if (r == NULL)
		return 1000000;
	if (r->Ss == NULL && r->Ds == NULL && r->Dj == NULL && r->Sj == NULL)
		return 0;

    int min = 9999;

	int niv_Ss = niv_frunze(r->Ss);
	if (min > niv_Ss)
        min = niv_Ss;

	int niv_Ds = niv_frunze(r->Ds);
	if (min > niv_Ds)
        min = niv_Ds;

    int niv_Dj = niv_frunze(r->Dj);
	if (min > niv_Dj)
        min = niv_Dj;

    int niv_Sj = niv_frunze(r->Sj);
	if (min > niv_Sj)
        min = niv_Sj;

	return 1 + min;
}

void compreseazaArbore(FILE *out, TArb r, int nivel) {
    if (r == NULL) return;

    if (nivel == 0 ) {
        if (r->tip == 1) {
            fwrite(&r->tip, sizeof(char), 1, out);
            fwrite(&r->info.rs, sizeof(char), 1, out);
            fwrite(&r->info.vr, sizeof(char), 1, out);
            fwrite(&r->info.ab, sizeof(char), 1, out);
        } else {
            fwrite(&r->tip, sizeof(char), 1, out);
        }
    } else {
        compreseazaArbore(out, r->Ss, nivel - 1);
        compreseazaArbore(out, r->Ds, nivel - 1);
        compreseazaArbore(out, r->Dj, nivel - 1);
        compreseazaArbore(out, r->Sj, nivel - 1);
    }
}
