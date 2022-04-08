#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

int L;
int H;
FILE * filePtrIn;
FILE * filePtrOut;

int main(int argc, char *argv[]) {

        struct timeval start, end;
        gettimeofday(&start,NULL);

        filePtrIn = fopen("P1Input.txt","r");
        if(filePtrIn == NULL) {
                printf("Error: File open failed.\n");
                exit(1);
        }

        filePtrOut = fopen("P1Part1.txt","w");
        if(filePtrOut == NULL) {
                printf("Error: File open failed.\n");
                exit(1);
        }

        fprintf(filePtrOut,"Hi I'm process %d and my parent is %d.\n",getpid(),getppid());
        L = atoi(argv[1]);
        H = atoi(argv[2]);

        if(L < 3000) {
                fprintf(filePtrOut,"Error: Argument L should be greater than or equal to 3000.\n");
                exit(1);
        }
        if(H <= 0 || H > 10) {
                fprintf(filePtrOut,"Error: Argument H should be greater than 0 and less than or equal to 10.\n");
                exit(1);
        }
        
        int *numList = (int*) malloc(L*sizeof(int));
        for(int i=0; i<L; i++) {
                fscanf(filePtrIn,"%d",&numList[i]);
        }

        int maxNum = 0;
        float avg = 0;
        int * keyLoc = (int*) malloc(H*sizeof(int));
        int currKey = 0;

        for(int i=0;i<L;i++) {
                if(numList[i] > maxNum) {
                        maxNum = numList[i];
                }
                avg += numList[i]/((float)L);
                if(numList[i] == -1) {
                        keyLoc[currKey] = i;
                        currKey++;
                }
                if(currKey == H) {
                        break;
                }
        }

        fprintf(filePtrOut,"Max = %d, Avg = %f.\n\n",maxNum,avg);
        for(int i=0;i<currKey;i++) {
                fprintf(filePtrOut,"Hi I am process %d and I found the hidden key in position A[%d].\n",getpid(),keyLoc[i]);
        }

        free(numList);
        free(keyLoc);

        gettimeofday(&end,NULL);
        float runTime = (end.tv_usec - start.tv_usec)/1000000.0;

        printf("Time Elapsed: %f.\n",runTime);

        return 0;
}
