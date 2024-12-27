void blockOfDeltaSigmaTo32 (unsigned long long *bitStream, int* outPtr, int outBufferLength);
void longBitStreamTo32values (unsigned long long bs64, int* outPtr);
void cicFilter16ForLongBitStream (unsigned long long bitStream, int* outPtr);
int convertBsFileTo32IntRawFile (char* cpInputFileName, char* cpOutputFileName, int* dipBuffer, int iLengthOfBuffer);