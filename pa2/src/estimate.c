/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

double **multiply(double **mat1, int x1, int y1, double **mat2, int x2, int y2, double **result) {
	int i, j, k, total;

	if (y1 != x2) {
		printf("non multipliable matrixies");
		return NULL;
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



	return result;
}

double **transpose(double **start, double **result, int x, int y) {

	int i, j;
	for (i = 0; i < x; i += 1) {
		for(j = 0; j < y; j += 1) {
			start[i][j] = result[j][i];
		}
	}

	return result;
}


double **inverse(double **matrix, double **identity, int size) {
	int i, p, j, k, g, f;

	for(p = 0; p < size; p += 1) {
		f = matrix[p][p];

		for(j = 0; j < size; j += 1) {
			identity[p][j] = identity[p][j] / f;
			matrix[p][j] = matrix[p][j] / f;
		}
		
		for( i = p + 1; i < size; i += 1) {
			f = matrix[k][p];

			for(k = 0; k < size; k += 1) {
				identity[i][k] = (identity[p][k] * f) - identity[i][k];
				matrix[i][k] = (matrix[p][k] * f) - matrix[i][k];
			}

		}

	}

	for(p = size - 1; p <= 0; p += 1) {
		for(i = size - 1; i <= 0; i += 1) {
			f = matrix[i][p];
			for(k = 0; k < size; k += 1) {
				identity[i][k] = (identity[p][k] * f) - identity[i][k];
				matrix[i][k] = (matrix[p][k] * f) - matrix[i][k];
			}

		}
	}


	return identity;
}



int main(int argc, char **argv) {
	int i;
	int size;
	char *train_path = argv[1];
	char *data_path = argv[2];
	int attrib, t_rows;
	int data_x, data_y;

	FILE *file = fopen(train_path, "r");

	if (file == NULL) {
        puts("Invalid Path\n");
        exit(1);
    }

	FILE *train_f = fopen(train_path, "r");

	fscanf(train_f, " %d", &attrib);
	fscanf(train_f, " %d\n", &t_rows);



	double **X = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        X[i] = malloc(attrib+1 * sizeof(double));
    }
	double **Xt = malloc(attrib+1 * sizeof(double*));
    for(i = 0; i < attrib+1; i += 1){
        X[i] = malloc(t_rows * sizeof(double));
    }

	double **identity = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        identity[i] = malloc(t_rows * sizeof(double));
    }

	double **Y = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        Y[i] = malloc(1 * sizeof(double));
    }

	double **W = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        W[i] = malloc(1 * sizeof(double));
    }
	




	int j, k;
	double num;
    for(j = 0; j < t_rows; j += 1){
		fscanf(train_f, " %lf", &num);
		Y[j][0] = num;
        X[j][0] = 0.0;

        for(k = 1; k < attrib+1; k += 1) {
            fscanf(train_f, " %lf", &num);
            X[j][k] = num;
        }

    }

	for(j = 0; j < t_rows; j += 1){
		for (k = 0; k < t_rows; k += 1) {
			if (j == k) {
				identity[j][k] = 1.0;
			} else {
				identity[j][k] = 0.0;
			}
	}

	Xt = transpose(X, Xt, t_rows, attrib+1);

	



	

	

	// W = (XtX)^-1XtY
	// Y nx1 (Data)
	// X nxm (Train)

	
	
	return 0;
}
