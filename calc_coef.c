#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	FILE * myFile;
	float coefsArray[64];
		const float pi = 3.14159265358979323846264;
	const float step =pi / 65.0000000000;
	float acc = step;
    float sumOfCoefs, result;
	sumOfCoefs = 0.000000000000000000;
	myFile = fopen("coefs.txt","w");
	for (int a; a<64; a++) {
		 result = sin(acc) / 41.372217072;
		 coefsArray[a] = result;
		 fprintf(myFile,"%.9f \n", result);
		 acc += step;
		 sumOfCoefs += result;
	}
	
	fprintf(myFile,"%.12f \n",sumOfCoefs);
	sumOfCoefs=0.00000000000000000;
	for (int s1=0; s1<64; s1++) {
		result = coefsArray[s1] * 65535.00000000;
		coefsArray[s1] = result;
		sumOfCoefs += coefsArray[s1];
		coefsArray[s1] = result;
		fprintf(myFile,"%.9f \n", result);
		
	}
	fprintf(myFile,"%.12f \n",sumOfCoefs);
	
	fclose(myFile);
  


	
	return 0;
}