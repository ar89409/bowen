#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MATERIAL	1
#define DELTA_X		0.1
#define IMP 		1e18
#define INTER 		1e15
#define TIME		3

int main() {
	double mat = MATERIAL;
	double dx = DELTA_X;
	double Imp = IMP;
	double Inter = INTER;
	double dt = 1;
	double k = 8.62e-5;
	double T = 1373;
	double Dix0 = 2.22e8;
	double Dixe = 3.46;
	double Dip0 = 4.23e9;
	double Dipe = 3.46;
	double Dix = Dix0*exp(-1*Dixe/(k*T));
	double Dip = Dip0*exp(-1*Dipe/(k*T));
	double m0 = 9.11e-31;
	double mn = 1.08;
	double mp = 0.56;
	double Eg = 1.12;
	double h = 4.135e-15;
	double PI = 3.14159;
	double ni = 2*(pow(((2*PI*k*T)/(h*h)),1.5))*(pow((mn*mp*m0),0.75))*(exp((-1*Eg)/(2*k*T)));
	double D = 1.45e-3;
	int time = TIME;
	int matSize = mat/dx;
	
	double curr_array[matSize];
	double next_array[matSize];
	double gaussianResult[matSize];
	double gaussianMatrix[matSize][matSize];

	memset(curr_array, 0, matSize*sizeof(double));

	for(int i = 0; i < matSize; i++) {
		if(i < (matSize / 2))
			curr_array[i] = Imp;
		else
			curr_array[i] = Inter;
	}
	
	for(int t = 1; t < time; t++) {
		printf("========= origin concentration of material =========\n");
		for(int i = 0; i < matSize; i++){
			printf("%f\n", curr_array[i]);
		}

		memset(next_array, 0, matSize*sizeof(double));
		for(int i = 0; i < matSize; i++) {
			if (i == 0)
				next_array[i] = Imp;
			else if (i == (matSize-1))
				next_array[i] = Inter;
			else
				next_array[i] = (curr_array[i-1]*(-1)) + (((2*dx*dx)/(D*dt)+2)*curr_array[i]-curr_array[i+1]);
		}
		
		printf("========= calculated contentraion of material =========\n");
		for(int i = 0; i < matSize; i++){
			printf("%f\n", next_array[i]);
		}

		memset(gaussianResult, 0, matSize*sizeof(double));
		for(int i = 0; i < matSize; i++) {
			if(i == 0)
				gaussianResult[i] = Imp;
			else if(i == (matSize-1))
				gaussianResult[i] = Inter;
			else
				gaussianResult[i] = next_array[i];
		}

		memset(gaussianMatrix, 0, matSize*matSize*sizeof(double));
		for(int i = 0; i < matSize; i++) {
			if(i == 0) {
				gaussianMatrix[0][0] = 1;
			} else if (i == (matSize-1)) {
				gaussianMatrix[matSize-1][matSize-1] = 1;
			} else {
				gaussianMatrix[i][i-1] = (-1)*((D*dt)/(2*dx*dx));
				gaussianMatrix[i][i] = (1+(D*dt)/(dx*dx));
				gaussianMatrix[i][i+1] = (-1)*((D*dt)/(2*dx*dx));
			}
		}

		for(int i = 0; i < matSize; i++) {
			for(int j = 0; j < matSize; j++) {
				printf("%.2f, ", gaussianMatrix[i][j]);
			}
			printf("%.2f\n", gaussianResult[i]);
		}

		for(int i = 1; i < (matSize-1); i++) {
			int pos = 0;
			double first = 0;
			for(int j = 0; j < matSize; j++) {
				if(gaussianMatrix[i][j] != 0) {
					first = gaussianMatrix[i][j];
					pos = j;
					break;	
				}
			}

			for(int j = pos; j < (pos+3); j++) {
				gaussianMatrix[i][j] /= first;
			}
			gaussianResult[i] /= first;
			
			for(int j = pos; j < (pos+3); j++) {
				gaussianMatrix[i][j] -= gaussianMatrix[i-1][j];
			}
			gaussianResult[i] -= gaussianResult[i-1];

			for(int j = 0; j < matSize; j++) {
				if(gaussianMatrix[i][j] != 0) {
					first = gaussianMatrix[i][j];
					pos = j;
					break;	
				}
			}

			for(int j = pos; j < (pos+2); j++) {
				gaussianMatrix[i][j] /= first;
			}
			gaussianResult[i] /= first;

		}
		
		for(int i = (matSize-2); i > 0; i--) {
			int pos = 0;
			double last = 0;
			for(int j = (matSize-1); j >= 0; j--) {
				if(gaussianMatrix[i][j] != 0) {
					last = gaussianMatrix[i][j];
					pos = j;
					break;	
				}
			}
			
			for(int j = pos; j >= (pos-1); j--) {
				gaussianMatrix[i][j] -= gaussianMatrix[i+1][j]*last;
			}

			gaussianResult[i] -= gaussianResult[i+1]*last;
		}

		for(int i = 0; i < matSize; i++) {
			for(int j = 0; j < matSize; j++) {
				printf("%.2f, ", gaussianMatrix[i][j]);
			}
			printf("%.2f\n", gaussianResult[i]);
		}

		for(int i = 0; i < matSize; i++) {
			curr_array[i] = gaussianResult[i];	
		}					
	}
	return 0;
}
