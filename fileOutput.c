/*
Do I need a function to write a regular unsigned int to make sure it is little endian?
This will have functions for writing different values to a file and for finding size in bytes.
Do I need a custom section for external functions, globals, or possibly size of structs for
linking?
*/

#include <stdio.h>

int writeMagicNumber(FILE *fp)
{

}

int writeByte(FILE *fp, char byte)
{
    return fwrite(&byte, sizeof(char), 1, fp);
}

unsigned int varUIntSize(unsigned int number)
{
    /*Returns the number of bytes needed to store a 32 bit integer in the variable
     * width format. I expect checking against the limited number of constants
     * is the most efficient way to handle this.*/

    unsigned int returnValue = 1;

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

int writeVarUInt(FILE *fp, unsigned int number)
{
    /*Writes an unsigned integer to a file and returns the number of bytes.*/
	unsigned char output[10];
    int numberOfBytes = 0;
	do{
		output[numberOFBytes] = number & 127;
		number = number >> 7;
		if(numberOfBytes > 0 && number > 0)
		{
			output[numberOfBytes - 1] = output[numberOfBytes - 1] + 128;
		}
		numberOfBytes++;
	}while(number != 0);
	//write output to file
    fwrite(output, sizeof(char), numberOfBytes, fp);
    //return number of bytes written
	return numberOfBytes;
}

int writeParams(FILE *fp, unsigned int numberOfParams, char *paramArray)
{
	/*Currently limited to 127 parameters*/
	char output[127];
	int numberOfBytes;
	if(numberOfParams < 128)
	{
		numberOfBytes = fwrite(paramArray, sizeof(char), numberOfParams, fp);
	}
	else
	{
		numberOfBytes = -1;
	}
	return numberOfBytes;
}

