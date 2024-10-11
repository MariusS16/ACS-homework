// SLINCU MARIUS 315CB
#include "arb.h"

int main(int argc, char const *argv[]) {
    FILE *in = fopen(argv[3], "rb");
    FILE *out = fopen(argv[4], "wb");

    char tip[2];
    fscanf(in, "%s\n", tip);//citim tipul
    unsigned int dim, dimh;
    fscanf(in, "%d %d\n", &dim, &dimh); //citim dimensiunile
    char vm[3];
    fscanf(in, "%s", vm);//citim valoarea maxima
    char c;
    fread(&c, sizeof(char), 1, in);

    struct Color **matrice = calloc(dim, sizeof(Color*));

    for (int i = 0; i < dim; i++) {//citim matricea
        matrice[i] = calloc(dim, sizeof(Color));
        for (int j = 0; j < dimh; j++) {
            fread(&matrice[i][j].rs, sizeof(char), 1, in);
            fread(&matrice[i][j].vr, sizeof(char), 1, in);
            fread(&matrice[i][j].ab, sizeof(char), 1, in);
        }
    }

    int factor = atoi(argv[2]);
    TArb arb;
    arb = arbore(matrice, 0, 0, dim, factor);
    
    if (strcmp(argv[1], "-c1") == 0) {
        int nivel = NrNiv(arb);
        fprintf(out, "%d\n", nivel);

        int NrFrz = frunze(arb);
        fprintf(out, "%d\n", NrFrz);

        int DimFr = niv_frunze(arb);
        fprintf(out, "%d\n", dim >> DimFr);
    } else if (strcmp(argv[1], "-c2") == 0) {
        fwrite(&dim, sizeof(int), 1, out);
        int nr = NrNiv(arb);
        for (int i = 0; i < nr; i++)
            compreseazaArbore(out, arb, i);
    }

    for (int i = 0; i < dim; i++) //dezalocam matricea
        free(matrice[i]);
    free(matrice);
    dezalocaArbore(arb);

    fclose(in);
    fclose(out);
	return 0;
}