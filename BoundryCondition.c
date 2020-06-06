#include <stdio.h>
#include <math.h>

#define _matT		3
#define _dx			0.001
#define _Imp 		pow(10,18)	
#define _Inter 		pow(10,15)

int main() {	
	double matT 	= _matT;
	double dx  		= _dx;
	double Imp 		= _Imp;
	double Inter	= _Inter;
	int	   arraySize = matT / dx;
	int	   mid 		= arraySize / 2;
	double BC[arraySize];
	
	for (int i=0; i<mid; i++) {
		BC[i] = Imp;
		//printf("%.2f\n", BC[i]);
	}

	for	(int i=mid; i < arraySize; i++){
		BC[i] =Inter;
        //printf("%.2f\n" , BC[i] );
	}

   // printf("%.2f\n%.2f\n", BC[mid-1], BC[mid]);
	


	return 0;
}
		
