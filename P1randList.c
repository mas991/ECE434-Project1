#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int L;
const int maxInt = 10000;
FILE * filePtr;

int main(int argc, char *argv[]) {

        filePtr = fopen("P1Input.txt","w");
        srand(time(NULL));

        if(filePtr == NULL) {
                printf("Error: File open failed.\n");
                exit(1);
        }

        if(argc < 2 || argc > 2) {
                fprintf(filePtr,"Error: Exactly 2 arguments required.\n");
                exit(1);
        }

        L = atoi(argv[1]);

        if(L < 3000) {
                fprintf(filePtr,"Error: Argument L should be greater than or equal to 3000.\n");
                exit(1);
        }

        int *randIntList = (int *) malloc(L*sizeof(int));
        int randInt = (rand() % L);

        for(int i=0; i<L; i++) {
                randIntList[i] = (rand() % maxInt) + 1;
        }

        for(int i=0; i<10; i++) {
                while(randIntList[randInt] == -1) {
                        randInt = (rand() % L);
                }
                randIntList[randInt] = -1;
        }

        for(int i=0; i<L; i++) {
                fprintf(filePtr,"%d\n",randIntList[i]);
        }

        return 0;
}