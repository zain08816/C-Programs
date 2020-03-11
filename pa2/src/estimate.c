/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

void print_mat(double **matrix, int row, int col) {
    int i, j;
    for (i = 0; i < row; i += 1) {
        for (j = 0; j < col; j += 1) {
            if (j == col - 1){
                printf("%.2f", matrix[i][j]);
                continue;
            }
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

double **multiply(double **mat1, int x1, int y1, double **mat2, int x2, int y2, double **result) {
	int i, j, k;
	double total;

	if (y1 != x2) {
		printf("non multipliable matrixies");
		return NULL;
	}
	// puts("multipying");

	for (i = 0; i < x1; i += 1){
		for(j = 0; j < y2; j += 1){
			for (k = 0; k < y1; k += 1){
				// printf(" %d ", k);
				total += mat1[i][k] * mat2[k][j];
				// printf("%.2f", mat1[i][k]);
				// printf("*%.2f=", mat2[k][j]);
				// printf("%.2f", total);
			}
			// printf("\n");
			// printf("%d ", total);
			// printf("%.2f\t", total);
			
			result[i][j] = total;
			total = 0.0;
		}
		// printf("\n");
    }

	return result;
}

double **transpose(double **start, double **result, int x, int y) {

	int i, j;
	for (i = 0; i < x; i += 1) {
		for(j = 0; j < y; j += 1) {
			// start[i][j] = result[j][i];
			result[i][j] = start[j][i];
		}
	}

	return result;
}


double **inverse(double **matrix, double **identity, int size) {
	int i, p, j, k;
	double f;
	// print_mat(identity, size, size);
	// print_mat(matrix, size, size);

	for(p = 0; p < size; p += 1) {
		f = matrix[p][p];

		for(j = 0; j < size; j += 1) {
			identity[p][j] = identity[p][j] / f;
			matrix[p][j] = matrix[p][j] / f;
		}
		
		for( i = p + 1; i < size; i += 1) {
			f = matrix[i][p];

			for(k = 0; k < size; k += 1) {
				identity[i][k] = identity[i][k] - (identity[p][k] * f);
				matrix[i][k] = matrix[i][k] - (matrix[p][k] * f);
			}

		}

	}

	puts("upper triangular");
	print_mat(matrix, size, size);
	puts(" ");
	print_mat(identity, size, size);

	for(p = size - 1; p >= 0; p -= 1) {
		for(i = p - 1; i >= 0; i -= 1) {
			f = matrix[i][p];
			for(k = 0; k < size; k += 1) {
				identity[i][k] = identity[i][k] - (identity[p][k] * f);
				matrix[i][k] = matrix[i][k] - (matrix[p][k] * f);
			}

		}

	}

	return identity;

}



void set_zero(double **matrix, int row, int col){
	int i, j;
	for (i = 0; i < row; i += 1) {
		for (j = 0; j < col; j += 1) {
			matrix[i][j] = 0;
		}
	}
	return;
}



int main(int argc, char **argv) {
	int i;
	char *train_path = argv[1];
	char *data_path = argv[2];
	int attrib, t_rows;
	int d_attrib, houses;
	char train_name[6];
	char data_name[5];

	FILE *data_f = fopen(data_path, "r");
	FILE *train_f = fopen(train_path, "r");

	if (train_f == NULL) {
        puts("Invalid Path\n");
        exit(1);
    }

	fscanf(data_f, " %s", data_name);
	fscanf(data_f, " %d", &d_attrib);
	fscanf(data_f, " %d\n", &houses);
	
	fscanf(train_f, " %s", train_name);
	fscanf(train_f, " %d", &attrib);
	fscanf(train_f, " %d\n", &t_rows);


	// allocating memory based on size
	double **X = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        X[i] = malloc((attrib+1) * sizeof(double));
    } // t_rows x attrib+1
	set_zero(X, t_rows, attrib+1);

	double **Xt = malloc((attrib+1) * sizeof(double*));
    for(i = 0; i < (attrib+1); i += 1){
        Xt[i] = malloc(t_rows * sizeof(double));
    } // attrib+1 x t_rows
	set_zero(Xt, attrib+1, t_rows);

	double **Y = malloc(t_rows * sizeof(double*));
    for(i = 0; i < t_rows; i += 1){
        Y[i] = malloc(1 * sizeof(double));
    } //t_rows x 1
	set_zero(Y, t_rows, 1);

	double **W = malloc((attrib+1) * sizeof(double*));
    for(i = 0; i < (attrib+1); i += 1){
        W[i] = malloc(1 * sizeof(double));
    } //attrib+1 x 1
	set_zero(W, attrib+1, 1);

	double **XtY = malloc((attrib+1) * sizeof(double*));
    for(i = 0; i < (attrib+1); i += 1){
        XtY[i] = malloc(1 * sizeof(double));
    } // attrib+1 x t_rows| t_rows x 1 = attrib+1 x 1
	set_zero(XtY, attrib+1, 1);

	double **XtX = malloc((attrib+1) * sizeof(double*));
    for(i = 0; i < (attrib+1); i += 1){
        XtX[i] = malloc(t_rows * sizeof(double));
    } // attrib+1 x t_rows | t_rows x attrib+1 = attrib+1 x attrib+1
	set_zero(XtX, attrib+1, attrib+1);

	double **identity = malloc((attrib+1) * sizeof(double*));
    for(i = 0; i < (attrib+1); i += 1){
        identity[i] = malloc((attrib+1) * sizeof(double));
    } // attrib+1 x attrib+1 
	set_zero(identity, attrib+1, attrib+1);

	double **X_prime = malloc((houses) * sizeof(double*));
    for(i = 0; i < (houses); i += 1){
        X_prime[i] = malloc((d_attrib+1) * sizeof(double));
    } // houses x d_attrib+1
	set_zero(X_prime, houses, d_attrib+1);

	double **Y_prime = malloc((houses) * sizeof(double*));
    for(i = 0; i < (houses); i += 1){
        Y_prime[i] = malloc(1 * sizeof(double));
    } // houses x 1
	set_zero(Y_prime, houses, 1);







	// copying data to proper locations
	int j, k;
	double num;
    for(j = 0; j < t_rows; j += 1){
		fscanf(train_f, " %lf", &num);
		// printf("%.0f\n", num);
		Y[j][0] = num;
        X[j][0] = 1.0;

        for(k = 1; k < attrib+1; k += 1) {
            fscanf(train_f, " %lf", &num);
			// printf("%.0f\n", num);
            X[j][k] = num;
        }
    }

	//setting up W_prime with data
	for (j = 0; j < houses; j += 1) {
		X_prime[j][0] = 1.0;
		for(k = 1; k < d_attrib+1; k += 1) {
            fscanf(data_f, " %lf", &num);
			// printf("%.0f\n", num);
            X_prime[j][k] = num;
		}	
	}
	


	for(j = 0; j < attrib+1; j += 1){
		for (k = 0; k < attrib+1; k += 1) {
			if (j == k) {
				identity[j][k] = 1.0;
			} else {
				identity[j][k] = 0.0;
			}
		}
	}
	


	//doing the math

	
	
	puts("identity");
	print_mat(identity, attrib+1, attrib+1);

	puts("X");
	print_mat(X, t_rows, attrib+1);

	puts("Y");
	print_mat(Y, t_rows, 1);

	puts("X_prime");
	print_mat(X_prime, houses, d_attrib+1);
	
	Xt = transpose(X, Xt, t_rows, attrib+1); // attrib+1 x t_rows
	puts("Xt");
	print_mat(Xt, attrib+1, t_rows);

	XtY = multiply(Xt, t_rows, attrib+1, Y, t_rows, 1, XtY); // attrib+1 x t_rows| t_rows x 1 = attrib+1 x 1
	puts("XtY");
	print_mat(XtY, attrib+1, 1);

	XtX = multiply(Xt, attrib+1, t_rows, X, t_rows, attrib+1, XtX); // attrib+1 x t_rows | t_rows x attrib+1 = attrib+1 x attrib+1
	puts("XtX");
	print_mat(XtX, attrib+1, attrib+1);

	identity = inverse(XtX, identity, attrib+1); // attrib+1 x attrib+1
	puts("identity");
	print_mat(identity, attrib+1, attrib+1);

	W = multiply(identity, attrib+1, attrib+1, XtY, attrib+1, 1, W); //attrib+1 x 1
	puts("W");
	print_mat(W, attrib+1, 1);

	
	Y_prime = multiply(X_prime, houses, d_attrib+1, W, attrib+1, 1, Y_prime); // houses x 1
	puts("Y_prime");
	print_mat(Y_prime, houses, 1);
	

	// W = (XtX)^-1XtY
	// Y nx1 (Data)
	// X nxm (Train)

	fclose(train_f);
	fclose(data_f);
	
	return 0;
}
