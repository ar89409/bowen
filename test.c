#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATERIAL	1
#define DELTA_X		0.1
#define IMP 		1e18
#define INTER 		1e15

//void printMatrix(double* matrix[row], int col, int row);
void divideFirst(double* array, int arraySize);
void eliminateUp2Down(double** gaussianMarix, int matSize, int pos);
void eliminateDown2Up(double** gaussianMarix, int matSize, int pos);

double* gaussianElimination(double* concentrationArray, int matSize);

int main() {
	double mat 		= MATERIAL;
	double dx  		= DELTA_X;
	double Imp 		= IMP;
	double Inter	= INTER;

	double dt=1;
	double k=8.62e-5;
	double T=1373;
	double Dix0=2.22e8;
	double Dixe=3.46;
	double Dip0=4.23e9;
	double Dipe=3.46;
	double Dix=Dix0*exp(-1*Dixe/(k*T));
	double Dip=Dip0*exp(-1*Dipe/(k*T));
	double m0=9.11e-31;
	double mn=1.08;
	double mp=0.56;
	double Eg=1.12;
	double h=4.135e-15;
	double PI=3.14159;
	double ni=2*(pow(((2*PI*k*T)/(h*h)),1.5))*(pow((mn*mp*m0),0.75))*(exp((-1*Eg)/(2*k*T)));
	double D=1.45e-3;
	int time = 2;
	int matSize	= mat/dx;
	
	// Alloc memories to matrix
	double **matrix = (double **)malloc(time * sizeof(double *));
	for (int i = 0; i < time; i++)
		matrix[i] = (double *)malloc(matSize * sizeof(double));

	// Init first concentration
	for(int i = 0; i < matSize; i++) {
		if(i < (matSize/2))
			matrix[0][i] = Imp;
		else
			matrix[0][i] = Inter;
	}
	
	for(int i = 1; i < time; i++) {
		// Calculate new concentation of material
		for(int j = 0; j < matSize; j++) {
			if (j == 0)
				matrix[i][j] = Imp;
			else if (j == (matSize-1))
				matrix[i][j] = Inter;
			else
				matrix[i][j] = (matrix[i-1][j-1]*(-1)) + (((2*dx*dx)/(D*dt)+2)*matrix[i-1][j]-matrix[i-1][j+1]);
		}

		// Gaussian elimination, put first concentration to gaussian elimination.
		gaussianElimination(matrix[0], matSize);
	}

	// Print all concentrations
	for(int i = 0; i < time; i++) {
		for(int j = 0; j < matSize; j++) 
			printf("%f\n", matrix[i][j]);
		printf("=====================\n");
	}
	return 0;
}


void divideFirst(double* array, int arraySize) {
	double first = 0;
	for (int i = 0; i < arraySize; i++) {
		if (array[i] != 0) {
			first = array[i];
			break;
		}
	}

	for (int i = 0; i < arraySize; i++) {
		array[i] /= first;
	}
}

void eliminateUp2Down(double** gaussianMatrix, int matSize, int pos) {
	if(pos > 0) {
		eliminateUp2Down(gaussianMatrix, matSize, pos-1);
		if (pos == (matSize-1)) {
			divideFirst(gaussianMatrix[pos-1], matSize);
		} else {
			divideFirst(gaussianMatrix[pos-1], matSize);
			for (int i = 0; i < matSize; i++) {
				gaussianMatrix[pos][i] -= gaussianMatrix[pos-1][i];
			}
		}
	}
}

void eliminateDown2Up(double** gaussianMatrix, int matSize, int pos) {
	if(pos > 0) {
        double last = 0;
        for (int i = matSize; i >= 0; i--) {
            if (gaussianMatrix[pos][i] != 0) {
                last = gaussianMatrix[pos][i];
                break;
            }
        }

		for (int i = 0; i < matSize; i++) {
			gaussianMatrix[pos][i] -= (gaussianMatrix[pos+1][i] * last);
		}
		eliminateDown2Up(gaussianMatrix, matSize, pos-1);
	}
}


double* gaussianElimination(double* concentrationArray, int matSize) {
	double **gaussianMatrix = (double **)malloc(matSize * sizeof(double *));
	for(int i = 0; i < matSize; i++)
		gaussianMatrix[i] = (double *)calloc(matSize, sizeof(double));

	for(int i = 0; i < matSize; i++) {
		if(i == 0) {
			gaussianMatrix[0][0] = 1;
		} else if (i == (matSize-1)) {
			gaussianMatrix[matSize-1][matSize-1] = 1;
		} else {
			gaussianMatrix[i][i-1] = 1;
			gaussianMatrix[i][i] = 5;
			gaussianMatrix[i][i+1] = 1;
		}
	}
    
    //eliminateUp2Down(gaussianMatrix, matSize, matSize);
    eliminateDown2Up(gaussianMatrix, matSize, matSize);
	for(int i = 0; i < matSize; i++) {
		for(int j = 0; j < matSize; j++)
			printf("%f, ", gaussianMatrix[i][j]);
		printf("\n");
	}

	return concentrationArray;
}
