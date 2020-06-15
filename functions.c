#include <stdio.h>
#include <stdlib.h>

void printMatrix(int* matrix, int m, int n) {
	for(int i = 0; i < m; i++) {
		printf("[");
		for(int j = 0; j < n; j++) {
			if (j != (n - 1)) 
				printf("%d, ", *(matrix + (n * i) + j));
			else
				printf("%d", *(matrix + (n * i) + j));
		}
		printf("]\n");
	}
	printf("\n");
}

void divideFirst(int* matrix, int m, int n) {
	int first = 0;
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			if(*(matrix + (n * i) + j) != 0) {
				first = *(matrix + (n * i) + j);
				break;
			}
		}

		for(int j = 0; j < n; j++) {
			*(matrix + (n * i) + j) /= first;
		}
	}
	
	printMatrix(matrix, m, n);
}

void eliminateUp2Down(int* matrix, int m, int n) {
	for (int i = 1; i < (m - 1); i++) {
		for (int j = 0; j < n; j++) {
			*(matrix + (n * i) + j) -= *(matrix + (n * (i - 1)) + j);
		}
	}

	printMatrix(matrix, m, n);
}

void eliminateDown2Up(int* matrix, int m, int n) {	
	for (int i = (m - 1); i > 1; i--) {
		for (int j = 0; j < n; j++) {
			*(matrix + (n * (i - 1)) + j) -= *(matrix + (n * i) + j);
		}
	}

	printMatrix(matrix, m, n);
}

int main(void) {
	int m = 5, n = 5;
	int matrix[5][5] = {
		{1,0,0,0,0},
		{-1,3,-1,0,0},
		{0,-1,4,-1,0},
		{0,0,-1,5,-1},
		{0,0,0,0,1}
	};

	printf("Matrix:\n");
	printMatrix((int *) matrix, m, n);
	
	printf("Divide First:\n");
	divideFirst((int *) matrix, m, n);

	printf("Eliminate Up to Down:\n");
	eliminateUp2Down((int *) matrix, m, n);
	
	printf("Eliminate Down to Up:\n");
	eliminateDown2Up((int *) matrix, m, n);

	return 0;
}
