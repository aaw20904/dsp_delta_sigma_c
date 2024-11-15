#include <stdio.h>
#include <stdlib.h>

/* EXAMPLE
riff_header='RIFF',
wav_size=size of wave portion
wave_header="WAVE"
fmt_header="fmt"
fmt_chunk_size=16
audio_format=1
num_channels=1
sample_rate=44100
byte_rate=88200
sample_alignment=2
bit_depth=16
data_header="data"
data_bytes=15987456
*/
typedef struct wav_header {
    // RIFF Header
    char riff_header[4]; // Contains "RIFF"
    int wav_size; // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"
    
    // Format Header
    char fmt_header[4]; // Contains "fmt " (includes trailing space)
    int fmt_chunk_size; // Should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate; // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth; // Number of bits per sample
    
    // Data
    char data_header[4]; // Contains "data"
    int data_bytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
unsigned int streamIIRFilter128(unsigned int bitOfStream);
unsigned int streamIIRFilter64(unsigned int bitOfStream);
unsigned int streamIIRFilter128_v2(unsigned int bitOfStream, unsigned int* pToOutBuffer);
unsigned int streamIIRFilter64_v2(unsigned int bitOfStream, unsigned int* pToOutBuffer);
void filter128Block(unsigned char* inputStream, unsigned int outputData);
int main(int argc, char *argv[]) {
	FILE* myAudioFile;
	FILE* templateFile;
	FILE* bitStreamFile;
	FILE* filteredFile;
	wav_header audio_header;
	unsigned long long bitStreamSize;
	unsigned int numOfBlocks;
	unsigned int numOfBlocksRemainder;
	const unsigned int bitStreamRate = 384000;
	unsigned int filteredHiSpeedData[64016];
	unsigned int inputStreamBuffer[64016];
	//open template of WAVE file header into structure
	templateFile = fopen("wav_header","rb");
	fread(&audio_header, sizeof(wav_header), 1, templateFile);
    fclose(templateFile);
    //correct wave data
    audio_header.sample_rate = 6000;
    audio_header.byte_rate = 12000;
    //open bit-stream file
    bitStreamFile = fopen("stream1.bin","rb");
    fseek (bitStreamFile, 0, SEEK_END);
    //size of delt-sigma file
    bitStreamSize = ftell(bitStreamFile);
    fseek (bitStreamFile, 0, SEEK_SET);
    //
    numOfBlocks = bitStreamSize / 64000;
    numOfBlocksRemainder = bitStreamSize % 64000;
    //cretae a file with filtered high-speed data
    filteredFile = fopen("filtered.bin","wb");
    for(int y1=0; y1<numOfBlocks;y1++){
    	// 1)read chunk of stream: read only 8000bytes, 
    	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    	// because  8000x8bit=64000;
    	//one bit - one 32bit word
    	fread(inputStreamBuffer,64000,1,bitStreamFile);
    	//2)filtering the one
    	//streamIIRFilter128_v2()
	}
	fclose(filteredFile); 
    
    //
    fclose(bitStreamFile);
	return 0;
}

void filter128Block(unsigned char* inputStream, unsigned int* outputData) {
	unsigned char inByte=0;
	const char msk = 0x01;
	for (int a3=0; a3 < 8000; a3++) {
		inByte = *inputStream; 
		for (int a4=0; a4<8; a4++ ) {
			streamIIRFilter64_v2((unsigned int)(inByte & msk), outputData);
			inByte >>= 1;
			outputData++;
		}
		inputStream++;
	}
}

//processing chunk of data -
//@blockSize - chunk size in bytes
void processChunkOfBitStream(unsigned char* streamBits, unsigned int* outSamples, unsigned int blockSize) {
	unsigned int  arg;
	unsigned char input;
	unsigned int streamIdx=0;
	for ( ;blockSize>0; blockSize--) {
		input = streamBits[streamIdx];
		for (int a=0; a <8;  a++ ) {
			//iterate bits in a byte
			arg = (unsigned int)(input & 0x01);
			  streamIIRFilter128_v2(arg, outSamples);
			outSamples++;
			input >>= 1;
		}
		streamIdx++;
	}
}

void process64Kbyte(unsigned long long* inBits, unsigned int *outSamples) {
	unsigned long long bits =0;
	unsigned int arg, inputIndex = 0;
	for (int a=0;a<8000;a++) {
		bits = inBits[inputIndex];
			for (int b=0; b < 64;  b++ ) {
			//iterate bits in a word
			arg = (unsigned int)(bits & 0x0000000000000001);
			streamIIRFilter128_v2(arg, outSamples);
			outSamples++;
			bits >>= 1;
		}
		inputIndex++;
	}
	
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
unsigned int streamIIRFilter128(unsigned int bitOfStream){


	static unsigned int bits [4]={0};
	unsigned int result=0;
	const unsigned int coefs[128]={1,2,4,6,8,11,14,17,21,26,31,37,44,52,61,71,82,94,108,122,
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
		//B) calculate fileter
		
		for (int y=0; y < 128; y++) {
			if (y < 32) {
				  if (bits[0] & (1<<y)) { 
				  	result += coefs[y];
				  }
			} else if ((y >= 32) && (y  < 64)){
				 if (bits[1] & (1<<(y-32))) { 
				  	result += coefs[y];
				  }
			} else if ((y >= 64) && (y  < 96)) {
				if (bits[2] & (1<<(y-64))) { 
				  	result += coefs[y];
				  }
			} else if (y >= 96) {
				if (bits[3] & (1<<(y-96))) { 
			  	   result += coefs[y];
			    }
			}
		}
		return result;
		//printf("%d \n", result );
		//printf("%X, %X, %X, %X \n",bits[0], bits[1], bits[2], bits[3]);
		
}

/* 128 Tap Low Pass Kaiser                                                 
  Finite Impulse Response                                                     
  Sample Frequency = 384.0 KHz                                                
  Standard Form                                                               
  Arithmetic Precision = 4 Digits                                                                                                                          
  Pass Band Frequency = 3.000 KHz  */

unsigned int streamIIRFilter64(unsigned int bitOfStream){

	static unsigned int bits [2] = {0};
	unsigned int result=0;
	unsigned int bitMask = 0;
 
	const unsigned int coefs[]={24,36,49,66,85,106,131,159,189,222,258,297,338,381,
	426,472,519,567,615,662,709,753,796,836,874,907,937,963,983,999,1010,1015,1015,
	1010,999,983,963,937,907,874,836,796,753,709,662,615,567,519,472,426,381,338,297,
	258,222,189,159,131,106,85,66,49,36,24};
		bitOfStream &= 0x00000001;
		
		//1)shift word 3
		bits[1] <<= 1;
		
		//2)has the word 2 MSB bit?
		if (bits[0] & 0x80000000) {
			bits[1] |= 1; //apply bit to higher word
		}
		//2)apply input bit of stream to word 0
			bits[0] <<= 1;
		    bits[0] |= bitOfStream;
		//B) calculate fileter
		
		for (int y=0; y < 64; y++) {
			if (y < 32) {
				  if (bits[0] & (1<<y)) { 
				  	result += coefs[y];
				  }
			} else {
				
				if (bits[1] & (1<<(y-32))) { 
			  	   result += coefs[y];
			    }
			  
			}
		}
		
		
		return result;
		//printf("%X, %X \n",bits[0], bits[1]);
		//printf("%d \n", result);
		
}

/* 128 Tap Low Pass Kaiser                                                 
  Finite Impulse Response                                                     
  Sample Frequency = 384.0 KHz                                                
  Standard Form                                                               
  Arithmetic Precision = 4 Digits                                                                                                                          
  Pass Band Frequency = 3.000 KHz  */

unsigned int streamIIRFilter64_v2(unsigned int bitOfStream, unsigned int* pToOutBuffer){

	static unsigned long long bits = {0};
	unsigned int result=0;
	unsigned long long bitMask = 1;
 
	const unsigned int coefs[]={24,36,49,66,85,106,131,159,189,222,258,297,338,381,
	426,472,519,567,615,662,709,753,796,836,874,907,937,963,983,999,1010,1015,1015,
	1010,999,983,963,937,907,874,836,796,753,709,662,615,567,519,472,426,381,338,297,
	258,222,189,159,131,106,85,66,49,36,24};
	
		bitOfStream &= 0x00000001;
	    bits 	<<= 1;
	    bits |= bitOfStream;
	    for (int a=0; a<64; a++) {
		 
	    	if (bitMask & bits) {
	            result += coefs[a];		
			}
			bitMask <<= 1;
		}
		
		*pToOutBuffer = result;
		
		//printf("%X, %X \n",bits[0], bits[1]);
	//	printf("%d \n", result);
		
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
unsigned int streamIIRFilter128_v2(unsigned int bitOfStream, unsigned int* pToOutBuffer) {
	
	static unsigned long long bits0 = 0;
	static unsigned long long bits1 = 0;
	unsigned int counterIndex = 0;
	unsigned long long state = 0;
	
	unsigned int result=0;
	const unsigned int coefs[128]={1,2,4,6,8,11,14,17,21,26,31,37,44,52,61,71,82,94,108,122,
		138,156,174,194,216,238,262,287,313,341,369,399,430,461,493,526,560,594,628,663,697,
		732,767,801,834,868,900,931,962,991,1019,1046,1071,1094,1116,1136,1153,1169,1183,
		1195,1204,1210,1216,1218,1218,1216,1210,1204,1195,1183,1169,1153,1136,1116,1094,
		1071,1046,1019,991,962,931,900,868,834,801,767,732,697,663,628,594,560,526,493,
		461,430,399,369,341,313,287,262,238,216,194,174,156,138,122,108,94,82,71,61,52,
		44,37,31,26,21,17,14,11,8,6,4,2,1 };
		bitOfStream &= 0x00000001;
		
		//1) shifting high word
		bits1 <<= 1;
		//2)Is the MSB in low word = 1  ?
		 state  = bits0 & 0x8000000000000000; //extract MSB
		 state >>= 63; //move to LSB
		 bits1 |= state; //apply to high word
		 //3) shft low word and apply input bit-stream
		 bits0 <<= 1;
		 bits0 |= bitOfStream;
		//processing first 64bits
	 
		state = 1;
		for (int a=0; a<64; a++) {
	    	if (state & bits0) {
	            result += coefs[counterIndex];		
			}
			state <<= 1;
			counterIndex++;
		}
		//processing last 64bit
		state = 1;
		for (int b=0; b<64; b++) {
		 
	    	if (state & bits1) {
	            result += coefs[counterIndex];		
			}
			state <<= 1;
			counterIndex++;
		}
		
		*pToOutBuffer = result;
		
	 //	printf("%d \n", result );
//	printf("%llx, %llx \n",bits0,bits1);
		
}
