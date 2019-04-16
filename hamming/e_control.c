/* Functions for error controlling

Software Systems Project 2: Siddharth Garimella

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int * corrected_nybble(int* nybble) {
    int *c;
    c = malloc(10*sizeof(int));

    c[0] = nybble[0];
    c[1] = nybble[1];
    c[2] = nybble[2];
    c[4] = nybble[3];

    c[6] = nybble[0]^nybble[2]^nybble[4];
    c[5] = nybble[0]^nybble[1]^nybble[4];
    c[3] = nybble[0]^nybble[1]^nybble[2];

    return c;
}

void correct_codeword(int* codeword) {
    //int * p = malloc(7*sizeof(int));
    //memcpy(p, codeword, 7*sizeof(int));
    int c1 = codeword[6]^codeword[4]^codeword[2]^codeword[0];
    int c2 = codeword[5]^codeword[4]^codeword[1]^codeword[0];
    int c3 = codeword[3]^codeword[2]^codeword[1]^codeword[0];
    int c = c3*4 + c2*2 + c1;
    //printf("%d",c);
    if (c==0) {
	puts("\nNO ERROR DETECTED.");
    }
    else {
	if (codeword[7-c]==0) {
    	    codeword[7-c]=1;
    	} else {
    	    codeword[7-c]=0;
    	}
    	puts("\nERROR CORRECTED");
    }
}

int main(int argc, char** argv) {
    FILE *f = fopen(argv[1], "r");

    char xr;

    while(fscanf(f," %c", &xr) == 1)
    {
        int n = 8;
        int data[n];
	
	printf("\n");

        char token = xr;
        for( int i = 7; i >= 0; i-- )
        {
            //printf( "%d", ( token >> i ) & 1 ? 1 : 0 );
            data[7-i] = (token >> i) & 1 ? 1 : 0;
        }
	
	//printf(": ");

        int nybble1[4];
        int nybble2[4];
        int* codenybble1;
        int* codenybble2;

        for (int i = 0; i < 8; i++)
        {
            if (i < 4) {
                nybble1[i] = data[i];
            } else {
                nybble2[i-4] = data[i];
            }
        }

        codenybble1 = corrected_nybble(nybble1);
	codenybble2 = corrected_nybble(nybble2);

	for (int i = 0; i < 7; i++)
	{
	    printf("%d",codenybble1[i]);
	}
	printf(" ");
	for (int i = 0; i < 7; i++)
	{
	    printf("%d", codenybble2[i]);
	}
	
	//correct_codeword(codenybble1);
	//correct_codeword(codenybble2);

    }
    printf("\n");
}
