#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

int H;
int L;
int PNnum;
int fd[2];
int cd[2];
int final[2];
int len;
int cpidnum[10];
FILE *filePtrIn;
FILE *filePtrOut;

int childProcessCode(int index);
int *fileArray;
struct timeval start, end;

int main(int argc, char *argv[]) {

        gettimeofday(&start,NULL);
        pid_t pid;
        filePtrIn = fopen("P1Input.txt","r");
        if(filePtrIn == NULL) {
                printf("Error: File open failed1.\n");
                exit(1);
        }

        filePtrOut = fopen("P1Part2.txt","w");
        if(filePtrOut == NULL) {
                printf("Error: File open failed2.\n");
                exit(1);
        }
	
	L = atoi(argv[1]);
        H = atoi(argv[2]);
        PNnum = atoi(argv[3]);

        if(L < 3000) {
                fprintf(filePtrOut,"Error: Argument L should be greater than or equal to 3000.\n");
                exit(1);
        }

        if(H <= 0 || H > 10) {
                fprintf(filePtrOut,"Error: Argument H should be less than or equal to 10 and greater than 0.\n");
                exit(1);
        }

        fileArray = (int*) malloc(L*sizeof(int));

        for(int i=0;i<L;i++) {
                fscanf(filePtrIn,"%d\n",&fileArray[i]);
        }

        int offset = 0;
        int rec = L/(PNnum-1);
        int totalProc = 1;
        int procNum = 0;
        int isChild = 0;
        pipe(fd);
        pipe(cd);
        pipe(final);
        int parentPID;
        while(totalProc < PNnum) {
                totalProc++;
                pid = fork();
                if(pid < 0) {
                        fprintf(filePtrOut,"Something went wrong and child process could not be created.\n");
                        exit(1);
                }
                if(pid == 0) {
                        isChild = 1;
                        procNum = totalProc;
		}
		else {
                        break;
                }
        }

        fprintf(filePtrOut,"Hi I'm process %d and my parent is %d.\n",getpid(),getppid());

        if(isChild) {
                wait(NULL);
                childProcessCode(procNum);
                exit(0);
        }

        for(int w=0;w<PNnum-1;w++) {
                write(fd[1],&offset,sizeof(int));
                offset = offset + rec;
        }

        int numKeysFound = 0;
        int * keyInfo = (int*) malloc(2*H*sizeof(int));
        while(numKeysFound < H) {
                read(cd[0],&keyInfo[2*numKeysFound],sizeof(int));
                read(cd[0],&keyInfo[2*numKeysFound+1],sizeof(int));
                numKeysFound++;
        }

        float * newpackage = (float*) malloc(3*sizeof(float));
        int finalmaxnum = 0;
        float finalavg = 0;

        for(int r=0; r<PNnum-1;r++) {
                read(final[0],newpackage,3*sizeof(float));
                if((int)newpackage[0] > finalmaxnum) {
                        finalmaxnum = (int) newpackage[0];
                }
                finalavg += newpackage[1]/newpackage[2];
        }
        fprintf(filePtrOut,"Max = %d, Avg = %f.\n\n",finalmaxnum,finalavg);
        for(int i=0;i<H;i++) {
                fprintf(filePtrOut,"Hi I am process %d and I found the hidden key in position A[%d].\n",keyInfo[2*i],keyInfo[2*i+1]);
	}

        gettimeofday(&end,NULL);
        float runTime = (end.tv_usec-start.tv_usec)/(float)1000000;
        printf("Time elapsed: %f.\n",runTime);

        return 0;
}


int childProcessCode(int index) {
        int offset;
        int n = read(fd[0],&offset,sizeof(int));
        int linestoRead;
        int procID = getpid();
        linestoRead = L/(PNnum-1);
        if(offset == L/(PNnum-1)*(PNnum-2)) {
                linestoRead = L-offset;
        }

        float weightAvg;
        int maxNum = 0;
        int hiddenkeys[H];
        int countKeys = 0;
        int indice = 0;
        for(int x = offset; x < offset+linestoRead;x++) {
                if(fileArray[x] > maxNum) {
                        maxNum = fileArray[x];
                }
                if(fileArray[x] == -1) {
                        hiddenkeys[countKeys] = x;
                        countKeys++;
                        cpidnum[indice] = getpid();
                        index++;
                        write(cd[1],&procID,sizeof(int));
                        write(cd[1],&x,sizeof(int));
                }
                weightAvg = weightAvg + ((float)fileArray[x]);
        }

        float * newpackage = (float*) malloc(3*sizeof(int));
        newpackage[0] = (float) maxNum;
        newpackage[1] = weightAvg;
        newpackage[2] = (float) linestoRead;
        write(final[1],newpackage,3*sizeof(float));
}
