//processing a block of bit-stream to 32bit integer 
//when 0 then 0xC0000000
//when 1 then 0x40000000
//!!!!! output buffer`s length must be (inp64WordsCount * 64)
void blockOfDeltaSigmaTo32(unsigned long long *bitStream, int* outPtr, int iNumOfInp64Words) {
	 
	int *ptrToOutArray = outPtr;
	for (int iNumCnt=0; iNumCont<iNumOfInp64Words; iNumCnt++ ) {
		//processing 64bit from bitstream
		 longBitStreamTo32values(*bitStream, *ptrToOutArray);
		 //increment pointer to out integer 32bit array
		ptrToOutArray += 64;
		//increment pointer to bitstream
		bitStream++;
	}
  
}

int convertBsFileTo32IntRawFile (char* cpInputFileName, char* cpOutputFileName) {
	//open bit stream file 
	FILE* fileIn, fileOut;
	int result;
	unsigned int iFileLength;
	unsigned int iNumOfBsChunks;
	unsigned int iChunksRemainder;
	
	fileIn = fopen(cpInputFileName,"rb");
	if (fileIn == NULL) {
		printf("File $s not found! \n",cpInputFileName);
		return -1;
	}
	
	fileOut = fopen(cpOutputFileName,"wb");
	if (fileOut == NULL) {
		return -2;
	}
	
	//read bit stream length
	fseek(fileIn,0,SEEK_END);
	iFileLength = ftell(fileIn);
	fseek(fileIn,0,SEEK_SET);
	// 1 input byte => 8 output words (32bit_word)
	//calculate number of chunks and a remainder ???????????????????????????
	iNumOfBsChunks = 	(iFileLength / iLengthOfBuffer);
	iChunksRemainder = 	(iFileLength % iLengthOfBuffer);
	
	
}
//!!!!! output buffer`s length must be (inp64WordsCount * 64)
void blockOfDeltaSigmaCICfilterTo32 (unsigned long long *bitStream, int* outPtr, int iNumOfInp64Words) {

	int *ptrToOutArray = outPtr;
		for (int iNumCnt=0; iNumCnt<iNumOfInp64Words; iNumCnt++ ) {
		//processing 64bit from bitstream
		cicFilter16ForLongBitStream(*bitStream, *ptrToOutArray);
		 //increment pointer to out integer 32bit array
		ptrToOutArray += 64;
		//increment pointer to bitstream
		bitStream++;
	}
}


void cicFilter16ForLongBitStream (unsigned long long bitStream, int* outPtr) {
	//static int* combDelayBasePointer=0; //full pointers to memory
	unsigned char comb1IdxIn = 0;
	unsigned char comb1IdxOut = 1;
	unsigned char comb2IdxIn = 0;
	unsigned char comb2IdxOut = 1;
	unsigned char comb3IdxIn = 0;
	unsigned char  comb3IdxOut =1;
	
	static int comb1Samples [64] = {0}; //delay buffer
	static int comb2Samples [64] = {0}; //delay buffer
	static int comb3Samples [64] = {0}; //delay buffer
	
	int  comb1 = 0;
	int  comb2 = 0;
	int  comb3 = 0;
	
	static int acc1 = 0;
	static int acc2 = 0;
	static int acc3 = 0;
	const unsigned long long mask = 1;
	#define DELAY_SIZE 64  // Comb delay size -1 
 
	
	for (int x1=0; x1 < 64; x1++) {
			// processing of a bit from bit stream
	  /****first filter stage***/
	  /***acc1**/
		//1)Accumulators section:
	      acc1 += bitStream & mask;
	      acc2 += acc1;
	      acc3 += acc2;
	      //2) combs section:
	      //A) first stage:
	      comb1 = acc3 - comb1Samples [ comb1IdxOut]; //comb out calculating
	      comb1Samples[comb1IdxIn] = acc3; //push input value to delay line
	      comb1IdxIn++;  //update indexes
		  comb1IdxOut++;  //update indexes
		  //wrap around implementation
		  comb1IdxIn = comb1IdxIn % DELAY_SIZE;
		  comb1IdxOut = comb1IdxOut % DELAY_SIZE;
		  //comb1 <<= 8;
		 //B) Second stage:
	      comb2 = comb1 - comb2Samples [ comb2IdxOut]; //comb out calculating
	      comb2Samples[comb2IdxIn] = comb1; //push input value to delay line
	      comb2IdxIn++;  //update indexes
		  comb2IdxOut++;  //update indexes
		  //wrap around implementation
		  comb2IdxIn = comb2IdxIn % DELAY_SIZE;
		  comb2IdxOut = comb2IdxOut % DELAY_SIZE;
		  //C) Thrid:
	      comb3 = comb2 - comb3Samples [ comb3IdxOut]; //comb out calculating
	      comb3Samples[comb3IdxIn] = comb2; //push input value to delay line
	      comb3IdxIn++;  //update indexes
		  comb3IdxOut++;  //update indexes
		  //wrap around implementation
		  comb3IdxIn = comb3IdxIn % DELAY_SIZE ;
		  comb3IdxOut = comb3IdxOut % DELAY_SIZE;
		  
		 //$ save new sample
		 *out = comb3;
		 //7)increment out opointer
		 out++;
		//8) shift bits of bitstream
		bitStream >>= 1;
	}
	
}
//converts 64bit chunk into 64 integer(32bit) values
//output array must have at least 64 "int" cells 
void longBitStreamTo32values(unsigned long long bs64, int* outPtr){
	const unsigned long long comparator = 0x00000001; 

  //b)compare bit0
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit1
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit2
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit3
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit4
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit5
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit6
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit7
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit8
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit9
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit10
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit11
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit12
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit13
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit14
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit15
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit16
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit17
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit218
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit19
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit20
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit21
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit22
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit23
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit24
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit25
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit26
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit27
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit28
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit29
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit30
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
   //b)compare bit31
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //------
    bs64 = *bitStream;
  //b)compare bit32
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit33
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit34
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit35
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit36
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit37
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit38
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit39
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit40
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit41
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit42
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit43
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit44
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit45
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit46
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit47
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit48
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit49
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit50
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit51
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit52
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit53
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit54
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit55
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit56
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
    //b)compare bit57
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit58
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit59
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit60
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit61
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  //b)compare bit62
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
   //b)compare bit63
  (comparator & bs64) ? (*outPtr=0x40000000) : (*outPtr=0xC0000000);
  bs64 >>= 1;
  outPtr++;
  
}