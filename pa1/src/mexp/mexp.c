#include <stdio.h>
#include <stdlib.h>

void print_mat(int size, int **matrix) {
    int i, j;
    for (i = 0; i < size; i += 1) {
        for (j = 0; j < size; j += 1) {
            if (j == size - 1){
                printf("%d", matrix[i][j]);
                continue;
            }
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

int multiply(int power, int size, int **matrix) {
    int i, j, k, h;
    int total = 0;

    int **result = malloc(size * sizeof(int*));
    for(i = 0; i < size; i += 1){
        result[i] = malloc(size * sizeof(int));
    }

    int **temp = malloc(size * sizeof(int*));
    for(i = 0; i < size; i += 1){
        temp[i] = malloc(size * sizeof(int));
    }



    for (i = 0; i < size; i += 1){
        for(j = 0; j < size; j += 1){
            total = 0;
            for(k = 0; k < size; k += 1){
                total += matrix[i][k] * matrix[k][j];
            }
            // printf("%d ", total);
            temp[i][j] = total;
        }
    }

    for(h = 0; h < power-2; h += 1) {
        for (i = 0; i < size; i += 1){
            for(j = 0; j < size; j += 1){
                total = 0;
                for(k = 0; k < size; k += 1){
                    total += temp[i][k] * matrix[k][j];
                }
                // printf("%d ", total);
                result[i][j] = total;
            }
        }
        for (j = 0; j < size; j += 1){
            for (k = 0; k < size; k += 1){
                temp[j][k] = result[j][k];
            }
        }
    }

    print_mat(size, temp);

    for(i = 0; i < size; i += 1) {
        free(temp[i]);
        free(result[i]);
    }
    free(temp);
    free(result);

    return 0;
}

void print_identity(int size) {
    int i, j;
    for (i = 0; i < size; i += 1) {
        for (j = 0; j < size; j += 1) {
            if (i == j){
                if (j == size - 1){
                    printf("%d", 1);
                    continue;
                }
                printf("%d ", 1);
                continue;
            }
            if (j == size - 1){
                printf("%d", 0);
                continue;
            }
            printf("%d ", 0);
        }
        printf("\n");
    }
    return;
}


int main(int argc, char **argv) {
    char *path = argv[1];
    int power = 0;
    int size = 0;
    FILE *file = fopen(path, "r");
    int i;

    

    if (file == NULL) {
        puts("Invalid Path");
        exit(1);
    }

    fscanf(file, "%d\n", &size);

    int **matrix = malloc(size * sizeof(int*));
    for(i = 0; i < size; i += 1){
        matrix[i] = malloc(size * sizeof(int));
    }

    int j, k, num;
    for(j = 0; j < size; j += 1){
        for(k = 0; k < size; k += 1) {
            fscanf(file, "%d ", &num);
            matrix[j][k] = num;
        }
    }

    fscanf(file, "%d", &power);


    if (power == 0) {
        print_identity(size);
        fclose(file);
    } else if (power == 1) {
        print_mat(size, matrix);
        fclose(file);
    } else {
        multiply(power, size, matrix);
        fclose(file);
    }

    for(i = 0; i < size; i += 1) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;


}