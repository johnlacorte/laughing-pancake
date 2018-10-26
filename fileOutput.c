/*
Do I need a function to write a regular unsigned int to make sure it is little endian?
This will have functions for writing different values to a file and for finding size in bytes.
Do I need a custom section for external functions, globals, or possibly size of structs for
linking?
*/

#include "fileOutput.h"

size_t writeMagicNumber(FILE *fp)
{
    char magicNumber[] = {0x0, 0x61, 0x73, 0x6d};
    return fwrite(magicNumber, sizeof(char), 4, fp);
}

size_t writeByte(FILE *fp, unsigned char byte)
{
    return fwrite(&byte, sizeof(char), 1, fp);
}

size_t varUIntSize(unsigned int number)
{
    /*Returns the number of bytes needed to store a 32 bit integer in the variable
     * width format. I expect checking against the limited number of constants
     * is the most efficient way to handle this.*/

    size_t returnValue = 1;

    if(number > 127)
    {
        returnValue++;
        if(number > 16383)
        {
            returnValue++;
            if(number > 2097151)
            {
                returnValue++;
                if(number > 268435455)
                {
                    returnValue++;
                }
             }
        }
    }
    return returnValue;
}

size_t writeVarUInt(FILE *fp, unsigned int number)
{
    /*Writes an unsigned integer to a file and returns the number of bytes.*/
	unsigned char output[10];
    int numberOfBytes = 0;
    size_t bytesWritten;
    do{
		output[numberOfBytes] = number & 127;
		number = number >> 7;
		if(numberOfBytes > 0)
		{
			output[numberOfBytes - 1] = output[numberOfBytes - 1] + 128;
        }
		numberOfBytes++;
	}while(number != 0);
	//write output to file
    bytesWritten = fwrite(output, sizeof(char), numberOfBytes, fp);
    //return number of bytes written
	return bytesWritten;
}

size_t writeByteArray(FILE *fp, unsigned int numberOfBytes, unsigned char *byteArray)
{
	/*Currently limited to 127 parameters*/
	unsigned char output[127];
	size_t bytesWritten;
	if(numberOfBytes < 128)
	{
		bytesWritten = fwrite(byteArray, sizeof(char), numberOfBytes, fp);
	}
	else
	{
		bytesWritten = 0;
	}
	return numberOfBytes;
}

