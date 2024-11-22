#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
  const float znum[64] = {
    7.35831194e-04,1.08438537e-03,1.50495161e-03,2.00279976e-03,2.58240134e-03,3.24726069e-03,3.99975613e-03,4.84099586e-03,5.770693e-03,6.78706395e-03,
        7.88675374e-03,9.06479161e-03,1.03145794e-02,1.16279142e-02,1.29950473e-02,1.44047775e-02,1.58445808e-02,1.73007721e-02,1.87586979e-02,2.02029564e-02,
        2.16176405e-02,2.29865991e-02,2.42937108e-02,2.55231651e-02,2.66597441e-02,2.76890989e-02,2.85980141e-02,2.93746552e-02,3.00087917e-02,3.04919924e-02,
        3.0817787e-02,3.09817912e-02,3.09817912e-02,3.0817787e-02,3.04919924e-02,3.00087917e-02,2.93746552e-02,2.85980141e-02,2.76890989e-02,2.66597441e-02,
        2.55231651e-02,2.42937108e-02,2.29865991e-02,2.16176405e-02,2.02029564e-02,1.87586979e-02,1.73007721e-02,1.58445808e-02,1.44047775e-02,1.29950473e-02,
        1.16279142e-02,1.03145794e-02,9.06479161e-03,7.88675374e-03,6.78706395e-03,5.770693e-03,4.84099586e-03,3.99975613e-03,3.24726069e-03,2.58240134e-03,
        2.00279976e-03,1.50495161e-03,1.08438537e-03,7.35831194e-04
    };
int main(int argc, char *argv[]) {
	FILE * myFile;
	unsigned int tmp1, intAcc;
	float coefsArray[64];
	const float pi = 3.14159265358979323846264;
	const float step =pi / 65.0000000000;
	float acc = step;
    float sumOfCoefs, result;
	sumOfCoefs = 0.000000000000000000;
	myFile = fopen("coefs.txt","w");
	for (int a=0; a<64; a++) {
	
		 sumOfCoefs += znum[a];
	}
	
	fprintf(myFile,"SumCoef %.16f \n",sumOfCoefs);
	sumOfCoefs=0.00000000000000000;
	intAcc=0;
	
	for (int s1=0; s1<64; s1++) {
		printf("%.9f \n",coefsArray[s1]);
		result = znum[s1] * 16777210.00000000; // 16777215
		//coefsArray[s1] = result;
		tmp1 = (unsigned int)round(result);
		//sumOfCoefs += coefsArray[s1];
		//coefsArray[s1] = result;
		intAcc += tmp1;
		fprintf(myFile,"%d, \n", tmp1);
		//fprintf(myFile,"%.9f , \n", result);
		
	}
	fprintf(myFile, "%d \n",intAcc);
//	fprintf(myFile,"%.16f \n",sumOfCoefs);
	
	fclose(myFile);
  


	
	return 0;
}