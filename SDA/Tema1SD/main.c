#include "lista2.h"

int main() {
	struct TBanda *B = calloc(1, sizeof(struct TBanda));
	struct TCoada *C = calloc(1, sizeof(struct TCoada));
	struct TStiva *SUndo = calloc(1, sizeof(struct TStiva));
	struct TStiva *SRedo = calloc(1, sizeof(struct TStiva));
	B->santinela = InitLista2();
	B->deget = B->santinela->urm;
	C = InitQ();
	SUndo = InitS();
	SRedo = InitS();
	FILE *fp;
	fp = fopen("tema1.in", "r");
	FILE *out = fopen("tema1.out", "w");

	int nr;
	char op[100];
	fgets(op, sizeof(op), fp);
	nr = atoi(op);

	for (int i = 0; i < nr; i++) {
		fgets(op, sizeof(op), fp);
		op[strcspn(op, "\n")] = '\0';

		if (strcmp("SHOW", op) == 0) {
			SHOW(B, out);
		}

		if (strcmp("SHOW_CURRENT", op) == 0) {
			SHOW_CURRENT(B, out);
		}

		if (strncmp(op, "EXECUTE", strlen("EXECUTE")) == 0) {
			EXECUTE(&C, &B, &SUndo, out);
		}

		if (strcmp("SHOW", op) && strcmp("SHOW_CURRENT", op) && strcmp("EXECUTE", op) && strcmp("UNDO", op) && strcmp("REDO", op)) {
			IntrQ(&C, op);
		}

		if (strcmp("UNDO", op) == 0) {
			IntrS(&SRedo, B->deget);
			B->deget = SUndo->inc->info;
			ExtrS(&SUndo);
		}

		if (strcmp("REDO", op) == 0) {
			IntrS(&SUndo, B->deget);
			B->deget = SRedo->inc->info;
			ExtrS(&SRedo);
		}
	}
	fclose(fp);
	fclose(out);
	return 0;
}