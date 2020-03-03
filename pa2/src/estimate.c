/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

int multiply(double **mat1, int x1, int y1, double **mat2, int x2, int y2, double **result) {
	int i, j, k, total;

	if (y1 != x2) {
		printf("non multipliable matrixies");
		return 1;
	}

	for (i = 0; i < x1; i += 1){
            for(j = 0; j < y2; j += 1){
                total = 0;
                for (k = 0; k < y1; k += 1){
                    total += mat1[i][k] * mat2[k][j];
                }
                // printf("%d ", total);
                result[i][j] = total;
            }
        }
	// for (j = 0; j < x1; j += 1){
	// 	for (k = 0; k < y2; k += 1){
	// 		mat1[j][k] = result[j][k];
	// 	}
	// }



	return 0;
}

int transpose(double **start, double **result) {

	return 0;
}

int inverse(double **matrix, double **identiy) {

	return 0;
}



int main(int argc, char **argv) {
	int i;
	int size;


	int **temp = malloc(size * sizeof(int*));
    for(i = 0; i < size; i += 1){
        temp[i] = malloc(size * sizeof(int));
    }

	
	
	return 0;
}
