#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
unsigned int streamIIRFilter(unsigned int bitOfStream);

int main(int argc, char *argv[]) {
	
	streamIIRFilter (0x00000001);
	for (int var=0; var<128; var++) {
		 streamIIRFilter (0x00000000);
	}
	return 0;
}
//converts bit-stream into 32-bit integer chunk of data
//This function save previous results in an static array
/* 128 Tap Low Pass Hamming                                                   
  Finite Impulse Response                                                     
  Sample Frequency = 384.0 KHz                                                
  Standard Form                                                               
  Arithmetic Precision = 4 Digits                                                                                                                          
  Pass Band Frequency = 3.000 KHz  */
  //sum all the coefs equals 65534
unsigned int streamIIRFilter(unsigned int bitOfStream){

	static unsigned int filterWindow [129]={0};
	static unsigned int bits [16]={0};
	
	const unsigned int coefs[]={1,2,4,6,8,11,14,17,21,26,31,37,44,52,61,71,82,94,108,122,
		138,156,174,194,216,238,262,287,313,341,369,399,430,461,493,526,560,594,628,663,697,
		732,767,801,834,868,900,931,962,991,1019,1046,1071,1094,1116,1136,1153,1169,1183,
		1195,1204,1210,1216,1218,1218,1216,1210,1204,1195,1183,1169,1153,1136,1116,1094,
		1071,1046,1019,991,962,931,900,868,834,801,767,732,697,663,628,594,560,526,493,
		461,430,399,369,341,313,287,262,238,216,194,174,156,138,122,108,94,82,71,61,52,
		44,37,31,26,21,17,14,11,8,6,4,2,1 };
		bitOfStream &= 0x00000001;
		
		//1)shift word 3
		bits[3] <<= 1;
		
		//1)has the word 2 MSB bit?
		if (bits[2] & 0x80000000) {
			bits[3] |= 1; //apply bit to higher word
			bits[2] <<= 1; //shift current 
		
		}else {
			bits[2] <<= 1;
		}
	
		//3)Has word 1 MSB?
		if (bits[1] & 0x80000000) {
		    bits[2] |= 1; //apply to higher
			bits[1] <<= 1; //shift current
			
		} else {
			bits[1] <<= 1;
		}
			//3)Has word 0 MSB?
		if (bits[0] & 0x80000000) {
			bits[0] <<= 1;
			bits[1] |= 1;
		} else {
			bits[0] <<= 1;
		}
			//2)apply input bit of stream to word 0
		
		bits[0] |= bitOfStream;
		
		
		printf("%X, %X, %X, %X \n",bits[0], bits[1], bits[2], bits[3]);
		
}

