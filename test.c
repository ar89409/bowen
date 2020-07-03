#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

#define MATERIAL	1
#define DELTA_X		0.1
#define IMP 		1e18
#define INTER 		1e15
#define TIME		5

void printMatrix(double* concentrationArray, double** gaussianMatrix, int matSize);
void divideFirst(double* concentrationArray, double* array, int arraySize, int pos);
void eliminateUp2Down(double* concentrationArray, double** gaussianMarix, int matSize, int pos);
void eliminateDown2Up(double* concentrationArray, double** gaussianMarix, int matSize, int pos);
void gaussianElimination(double* concentrationArray, double** gaussianMatrix, int matSize);

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
	int time = TIME;
	int matSize	= mat/dx;

	// Alloc memories to matrix
	double **matrix = (double **)malloc(time * sizeof(double *));
	for(int i = 0; i < time; i++)
		matrix[i] = (double *)malloc(matSize * sizeof(double));

	// Init first concentration
	for(int i = 0; i < matSize; i++) {
		if(i < (matSize/2))
			matrix[0][i] = Imp;
		else
			matrix[0][i] = Inter;
	}
	
	for(int t = 1; t < time; t++) {
		// Print origin concentration of material
		for(int i = 0; i < matSize; i++){
			printf("%f\n", matrix[t-1][i]);
		}
		printf("=================\n");

		// Calculate new concentration of material
		for(int l = 0; l < matSize; l++) {
			if (l == 0)
				matrix[t][l] = Imp;
			else if (l == (matSize-1))
				matrix[t][l] = Inter;
			else
				matrix[t][l] = (matrix[t-1][l-1]*(-1)) + (((2*dx*dx)/(D*dt)+2)*matrix[t-1][l]-matrix[t-1][l+1]);
		}
		
		// Print calculated contentraion of material
		for(int i = 0; i < matSize; i++){
			printf("%f\n", matrix[t][i]);
		}
		printf("=================\n");


		// Build gaussian matrix
		double **gaussianMatrix = (double **)malloc(matSize * sizeof(double *));
		for(int i = 0; i < matSize; i++)
			gaussianMatrix[i] = (double *)calloc(matSize, sizeof(double));

		for(int i = 0; i < matSize; i++) {
			if(i == 0) {
				// First item of the first colume is 1, else 0
				gaussianMatrix[0][0] = 1;
			} else if (i == (matSize-1)) {
				// Last item of the last colume is 1, else 0
				gaussianMatrix[matSize-1][matSize-1] = 1;
			} else {
				gaussianMatrix[i][i-1] = (-1)*((D*dt)/(2*dx*dx));
				gaussianMatrix[i][i] = (1+(D*dt)/(dx*dx));
				gaussianMatrix[i][i+1] = (-1)*((D*dt)/(2*dx*dx));
			}
		}

		// Gaussian elimination, put each concentration to gaussian elimination.
		gaussianElimination(matrix[t], gaussianMatrix, matSize);

		// Free gaussian matrix memory
		for(int i = 0; i < matSize; i++)
			free(gaussianMatrix[i]);
		free(gaussianMatrix);
	}
	
	/*
	// Print all concentrations
	for(int i = 0; i < time; i++) {
		for(int j = 0; j < matSize; j++) 
			printf("%f\n", matrix[i][j]);
		printf("=====================\n");
	}
	*/
	return 0;
}

void printMatrix(double* concentrationArray, double** gaussianMatrix, int matSize) {
	for (int i = 0; i < matSize; i++) {
		for (int j = 0; j < matSize; j++) {
			printf("%.2f, ", gaussianMatrix[i][j]);
		}
		printf("%.2f\n", concentrationArray[i]);
	}
	printf("====================\n");
}

void divideFirst(double* concentrationArray, double* array, int arraySize, int pos) {
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
	
	concentrationArray[pos] /= first;
}

void eliminateUp2Down(double* concentrationArray, double** gaussianMatrix, int matSize, int pos) {
	if (pos == (matSize-1)) {
		// Recursion end
		eliminateUp2Down(concentrationArray, gaussianMatrix, matSize, pos-1);
		
		// All elements divide first non zero element
		divideFirst(concentrationArray, gaussianMatrix[pos-1], matSize, pos-1);
		
		printMatrix(concentrationArray, gaussianMatrix, matSize);
	
	} else if (pos > 0) {
		// Recursion
		eliminateUp2Down(concentrationArray, gaussianMatrix, matSize, pos-1);
		
		printMatrix(concentrationArray, gaussianMatrix, matSize);
		
		// Make the first item of upper concentration is 1
		divideFirst(concentrationArray, gaussianMatrix[pos-1], matSize, pos-1);

		// Make the first item of current concentration is 1
		divideFirst(concentrationArray, gaussianMatrix[pos], matSize, pos);
		
		// Eliminate first element
		for (int i = 0; i < matSize; i++) {
			gaussianMatrix[pos][i] -= gaussianMatrix[pos-1][i];
		}

		// Calculate new concentration
		concentrationArray[pos] -= concentrationArray[pos-1];
	}
}

void eliminateDown2Up(double* concentrationArray, double** gaussianMatrix, int matSize, int pos) {
	if (pos == (matSize - 1)) {
		printMatrix(concentrationArray, gaussianMatrix, matSize);
		
		// Recursion end
		eliminateDown2Up(concentrationArray, gaussianMatrix, matSize, pos-1);
	} else if(pos > 0) {
		// Get last non zero 
        double last = 0;
        for (int i = (matSize - 1); i >= 0; i--) {
            if (gaussianMatrix[pos][i] != 0) {
                last = gaussianMatrix[pos][i];
                break;
            }
        }
		
		// Eliminate last item 
		for (int i = 0; i < matSize; i++) {
			gaussianMatrix[pos][i] -= (gaussianMatrix[pos+1][i] * last);
		}

		// Calculate new concentration
		concentrationArray[pos] -= (concentrationArray[pos+1] * last);
		
		printMatrix(concentrationArray, gaussianMatrix, matSize);
		
		// Recursion
		eliminateDown2Up(concentrationArray, gaussianMatrix, matSize, pos-1);
	}
}


void gaussianElimination(double* concentrationArray, double** gaussianMatrix, int matSize) {
	int pos = matSize - 1;
    eliminateUp2Down(concentrationArray, gaussianMatrix, matSize, pos);
    eliminateDown2Up(concentrationArray, gaussianMatrix, matSize, pos);
	
	/*
	for(int i = 0; i < matSize; i++) {
		for(int j = 0; j < matSize; j++)
			printf("%f, ", gaussianMatrix[i][j]);
		printf("\n");
	}
	*/
}
