#include <stdio.h>
#include <stdlib.h>

#define COL 5
#define ROW 5

void printMatrix(double matrix[][ROW]);
void checkFirst(double array[ROW]);
double getLastNonZero(double array[ROW]);
void eliminateUp2Down(double matrix[][ROW], int col);
void eliminateDown2Up(double matrix[][ROW], int col);
void gaussianElimination(double matrix[][ROW]);

int main(void) {
	
	double matrix[COL][ROW] = {
		{1,0,0,0,0},
		{1,3,1,0,0},
		{0,1,4,1,0},
		{0,0,1,5,1},
		{0,0,0,0,1}
	};
	
	printMatrix(matrix);
	gaussianElimination(matrix);
	
	return 0;
}

void printMatrix(double matrix[][ROW]) {
	for(int i = 0; i < COL; i++) {
		printf("[");
		for(int j = 0; j < ROW; j++) {
			if (j != (ROW - 1)) 
				printf("%f, ", matrix[i][j]);
			else
				printf("%f", matrix[i][j]);
		}
		printf("]\n");
	}
	printf("\n");
}

void checkFirst(double array[ROW]) {
	double index = 0;
	for (int i = 0; i < ROW; i++) {
		if (array[i] != 0) {
			index = array[i];
			break;
		}
	}

	for (int i = 0; i < ROW; i++) {
		array[i] /= index;
	}
}

double getLastNonZero(double array[ROW]) {
	double last = 0;
	for (int i = ROW-1; i >= 0; i--) {
		if (array[i] != 0) {
			return array[i];
		}
	}
}

void eliminateUp2Down(double matrix[][ROW], int col) {
	if(col > 0) {
		eliminateUp2Down(matrix, col-1);
		if (col == COL-1) {
			checkFirst(matrix[col-1]);
		} else {
			checkFirst(matrix[col-1]);
			for (int i = 0; i < ROW; i++) {
				matrix[col][i] -= matrix[col-1][i];
			}
		}
	}
}

void eliminateDown2Up(double matrix[][ROW], int col) {
	if(col > 0) {
		double lastNonZero = getLastNonZero(matrix[col]);
		for (int i = 0; i < ROW; i++) {
			matrix[col][i] -= (matrix[col+1][i] * lastNonZero);
		}
		eliminateDown2Up(matrix, col-1);
	}
}

void gaussianElimination(double matrix[][ROW]) {
	int col = COL-1;
	eliminateUp2Down(matrix, col);
	printMatrix(matrix);

	eliminateDown2Up(matrix, col-1);
	printMatrix(matrix);
}

