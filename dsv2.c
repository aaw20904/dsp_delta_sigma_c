#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
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
    /////
   
    // Data
    char data_header[4]; // Contains "data"
    int data_bytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

void cicThridOrder(unsigned long long bitStream, int* out);
int *rawHiSpeedAudio, *rawBitStream, *bitStreamPtr, rawAudioPtr;
unsigned long long lpStreambuffer;

int main(int argc, char *argv[]) {
	FILE* fileBitStream;
	FILE* fileRawData;
	int numOfBlocks =0;
	int streamLength =0;
	int rawAudioData32Length =0; //number of 32bit samples
	unsigned long long myTestData;
	 
		fileBitStream = fopen("stream1.bin","rb");
		if (!fileBitStream) {
			printf("Error opening file! \n");
			return -1;
		}
		
		//alocate 64000 integer cells
		rawHiSpeedAudio = malloc(256100);
		rawBitStream = malloc(8000); //8000_bytes*8_bits=64KBit
		//read stream size
		fseek(fileBitStream, 0, SEEK_END);
		streamLength = ftell(fileBitStream);
		fseek(fileBitStream,0,SEEK_SET);
		//calculate number of whole blocks (remainder ignored)
		numOfBlocks = streamLength  / 8000; //8000*8_bits=64_kBits
		//size of raw audio data
		rawAudioData32Length = streamLength * 8;
		
	    for (unsigned int blockCounter=0; blockCounter < numOfBlocks; ) {
	    	//read block of data (64Kbit)
	    	fread (rawBitStream,8000,1,fileBitStream);
	    	blockCounter += 8000;
	    	///processing a block (8000bytes) of bitstream
	    	lpStreambuffer = rawBitStream; 
	    	   for (int innerCntr=0; innerCntr < 8000; innerCntr++){
	    	   	
	    	   	
	    	   	
			   }
		}
 
	return 0;
}

void cicThridOrder(unsigned long long bitStream, int* out) {
	//static int* combDelayBasePointer=0; //full pointers to memory
	unsigned static char comb1IdxIn = 0;
	unsigned static char comb1IdxOut = 1;
	unsigned static char comb2IdxIn = 0;
	unsigned static char comb2IdxOut = 1;
	unsigned static char comb3IdxIn = 0;
	unsigned static char  comb3IdxOut =1;
	unsigned static char comb4IdxIn = 0;
	unsigned static char  comb4IdxOut =1;
	unsigned static char comb5IdxIn = 0;
	unsigned static char  comb5IdxOut =1;
	
	static int comb1Samples [64] = {0}; //delay buffer
	static int comb2Samples [64] = {0}; //delay buffer
	static int comb3Samples [64] = {0}; //delay buffer
	static int comb4Samples [64] = {0}; //delay buffer
	static int comb5Samples [64] = {0}; //delay buffer
	
	int  comb1 = 0;
	int  comb2 = 0;
	int  comb3 = 0;
		int  comb4 = 0;
		int  comb5 = 0;
	
	static int acc1 = 0;
	static int acc2 = 0;
	static int acc3 = 0;
	static int acc4 = 0;
	static int acc5 = 0;

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
	      
	      acc4 += acc3;
	      
	      acc5 += acc4;
	      //2) combs section:
	      //A) first stage:
	      
	      comb1 = acc5 - comb1Samples [ comb1IdxOut]; //comb out calculating
	      comb1Samples[comb1IdxIn] = acc5; //push input value to delay line
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
		  //D) forth
		  comb4 = comb3 - comb3Samples [ comb4IdxOut]; //comb out calculating
	      comb4Samples[comb4IdxIn] = comb3; //push input value to delay line
	      comb4IdxIn++;  //update indexes
		  comb4IdxOut++;  //update indexes
		  //wrap around implementation
		  comb4IdxIn = comb4IdxIn % DELAY_SIZE ;
		  comb4IdxOut = comb4IdxOut % DELAY_SIZE;
		  //E) fifth
		  comb5 = comb4 - comb5Samples [ comb5IdxOut]; //comb out calculating
	      comb5Samples[comb5IdxIn] = comb4; //push input value to delay line
	      comb5IdxIn++;  //update indexes
		  comb5IdxOut++;  //update indexes
		  //wrap around implementation
		  comb5IdxIn = comb5IdxIn % DELAY_SIZE ;
		  comb5IdxOut = comb5IdxOut % DELAY_SIZE;
		  
		 //$ save new sample
		 *out = comb5;
		 //7)increment out opointer
		 out++;
		//8) shift bits of bitstream
		bitStream >>= 1;
	}
	
}