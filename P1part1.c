#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int L; // Command Line Argument 1
int H; // Command Line Argument 2
FILE * filePtrIn; // Input File Pointer
FILE * filePtrOut; // Output File Pointer

int main(int argc, char *argv[]) {

        filePtrIn = fopen("P1Input.txt","r"); // Open file called "P1Input.txt" for reading
        if(filePtrIn == NULL) { // Error
                printf("Error: File open failed.\n");
                exit(1);
        }

        filePtrOut = fopen("P1Part1.txt","w"); // Open file called "P1Part1.txt" for writing
        if(filePtrOut == NULL) { // Error
                printf("Error: File open failed.\n");
                exit(1);
        }

        fprintf(filePtrOut,"Hi I'm process %d and my parent is %d.\n",getpid(),getppid()); // Print Process information
        L = atoi(argv[1]);
        H = atoi(argv[2]);

        if(L < 3000) { // L out of bounds
                fprintf(filePtrOut,"Error: Argument L should be greater than or equal to 3000.\n");
                exit(1);
        }
        if(H <= 0 || H > 10) { // H out of bounds
                fprintf(filePtrOut,"Error: Argument H should be greater than 0 and less than or equal to 10.\n");
                exit(1);
        }

        int *numList = (int*) malloc(L*sizeof(int)); // Array to hold input list of integers
        for(int i=0; i<L; i++) {
                fscanf(filePtrIn,"%d",&numList[i]); // Scan input file to populate array
        }
        
        int maxNum = 0; // Maximum recorded integer
        int sum = 0; // Sum of all integers
        int * keyLoc = (int*) malloc(H*sizeof(int)); // Location of hidden keys
        int currKey = 0; // Index for keyLoc

        for(int i=0;i<L;i++) {
                if(numList[i] > maxNum) {
                        maxNum = numList[i]; // Update maxNum
                }
                sum += numList[i]; // Update sum
                if(numList[i] == -1) { // Update keyLoc, currKey
                        keyLoc[currKey] = i;
                        currKey++;
                }
        }
        float avg = ((float)sum / (float)L); // Calculate Average

        fprintf(filePtrOut,"Max = %d, Avg = %f.\n\n",maxNum,avg); // Print Input file information
        for(int i=0;i<H;i++) { // Print key locations and who found each key
                fprintf(filePtrOut,"Hi I am process %d and I found the hidden key in position A[%d].\n",getpid(),keyLoc[i]);
        }
        
        free(numList); // Free memory
        free(keyLoc);
        
        return 0;
}
