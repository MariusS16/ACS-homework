// SLINCU MARIUS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Color {
    unsigned char rs;
    unsigned char vr;
    unsigned char ab;
}Color;

typedef struct ArbC {
    struct Color info;
    unsigned char tip;
    struct ArbC *Ss, *Ds, *Dj, *Sj;
} TNod, *TArb;

unsigned long long CalcMean(Color **matrice, int x, int y, unsigned int dim, Color *culori);
TArb arbore(Color **matrice, int x, int y, unsigned int dim, int factor);
int NrNiv(TArb r);
int frunze(TArb r);
int niv_frunze(TArb r);
void dezalocaArbore(TArb r);
void compreseazaArbore(FILE *out, TArb r, int nivel);