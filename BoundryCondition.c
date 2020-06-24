#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _matT		1
#define _dx			0.1
#define _Imp 		1e18
#define _Inter 		1e15

int main() {
	double matT 	= _matT;
	double dx  		= _dx;
	double Imp 		= _Imp;
	double Inter	= _Inter;
	int arraySize	= matT/dx;
	int	mid 		= arraySize / 2;
	int col			= arraySize;
	int row			= arraySize;
	double BC[arraySize];
	
	for (int i = 0; i < mid; i++) {
		BC[i] = Imp;
		//printf("%.2f\n", BC[i]);
	}

	for	(int i = mid; i < arraySize; i++){
		BC[i] =Inter;
        //printf("%.2f\n" , BC[i]);
	}

    // printf("%.2f\n%.2f\n", BC[mid-1], BC[mid]);
	
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
	
	double new_col[row];
	for (int i = 0; i < row; i++) {
		if (i == 0)
			new_col[i] = Imp;
		else if (i == row-1)
			new_col[i] = Inter;
		else
			new_col[i] = (-1*BC[i-1]) + (((2*dx*dx)/(D*dt)+2)*BC[i]) - BC[i+1];
	}
	for (int i = 0; i < row-2; i++)
		printf("%f\n", BC[i]);

	printf("========\n");

	for (int i = 0; i < row; i++)
		printf("%f\n", new_col[i]);

	// - BC[0] + (((2*dx*dx)/(D*dt)+2)*BC[1]) - BC[2] 
	return 0;
}

