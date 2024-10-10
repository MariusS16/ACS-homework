#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *in= fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    int i, n;
    fscanf(in, "%d %d", &i, &n);

    char line[1000], input[1000][3];
    int ii = 0, jj = 0;
    while (fscanf(in, "%s", line) != EOF){
        for (int j = 0; j < strlen(line); j++){
            input[ii][jj] = line[j];
        
            jj++;
        }
        ii++;
        jj = 0;
    }
    int clauze = 1;
    for (int h = 0; h < ii; h++){
        if(input[h][0] == '1' || input[h][0] == '2' || input[h][0] == '3' || input[h][0] == '4' || input[h][0] == '5' || input[h][0] == '6' || input[h][0] == '7' || input[h][0] == '8' || input[h][0] == '9')
            clauze++;
    }
    fprintf(out, "p cnf %d %d\n", n, clauze);
    char output[100][100];
    for (int k = 0; k < ii; k++){
        if (input[k][0] == 'N' && input[k][1] == 'O' && input[k][2] == 'T'){
            int input_gates_not[100];
            k++;
            int l = 0;
            int inputs = 0;
            while (input[k][0] != 'A' && input[k][0] != 'N' && input[k][0] != 'O' && input[k][0] != '\0'){
                if (strlen(input[k]) == 2){
                    input_gates_not[l++] = 10 * (input[k][0] - '0') + (input[k][1] - '0');
                    inputs++;
                }
                else{
                    input_gates_not[l++] = input[k][0] - '0';
                    inputs++;
                }
                k++;
            }
            k--;
            fprintf(out, "-%d -%d 0\n", input_gates_not[inputs - 2], input_gates_not[inputs - 1]);
            fprintf(out, "%d %d 0\n", input_gates_not[inputs - 2], input_gates_not[inputs - 1]);
        }

        if(input[k][0] == 'A' && input[k][1] == 'N' && input[k][2] == 'D'){
            int input_gates_and[100];
            k++;
            int l = 0;
            int inputs = 0;
            while (input[k][0] != 'A' && input[k][0] != 'N' && input[k][0] != 'O' && input[k][0] != '\0'){
                if (strlen(input[k]) == 2){
                    input_gates_and[l++] = 10 * (input[k][0] - '0') + (input[k][1] - '0');
                    inputs++;
                }
                else{
                    input_gates_and[l++] = input[k][0] - '0';
                    inputs++;
                }
                k++;
            }
            for (int m = 0; m < inputs - 1; m++) {
                fprintf(out, "-%d ", input_gates_and[m]);
            }
            fprintf(out, "%d 0\n", input_gates_and[inputs - 1]);

            for (int m = 0; m < inputs - 1; m++) {
                fprintf(out, "%d -%d 0\n", input_gates_and[m], input_gates_and[inputs - 1]);
            }
            k--;
        }

        if(input[k][0] == 'O' && input[k][1] == 'R'){
            int input_gates_or[1000];
            k++;
            int l = 0;
            int inputs = 0;
            while (input[k][0] != 'A' && input[k][0] != 'N' && input[k][0] != 'O' && input[k][0] != '\0'){
                if (strlen(input[k]) == 2){
                    input_gates_or[l++] = 10 * (input[k][0] - '0') + (input[k][1] - '0');\
                    inputs++;
                }
                else{
                    input_gates_or[l++] = input[k][0] - '0';
                    inputs++;
                }
                k++;
            }
            for (int m = 0; m < inputs - 1; m++) {
                fprintf(out, "%d ", input_gates_or[m]);
            }
            fprintf(out, "-%d 0\n", input_gates_or[inputs - 1]);

            for (int m = 0; m < inputs - 1; m++) {
                fprintf(out, "-%d %d 0\n", input_gates_or[m], input_gates_or[inputs - 1]);
            }
            k--;
        }
    }
    fprintf(out, "%d 0", n);
    fclose(in);
    fclose(out);
    return 0;
}