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
int PN;//THIS SHOULD BE INCREASED FOR BETTER ALGORITHM PERFORMANCE
int fd[2];
int cd[2];
int final[2];
int len;
int cpidnum[10];
FILE *filePtrIn;
FILE * filePtrOut;
//FUNCTION DECLARATIONS
int childProcessCode(int index);
int *fileArray;//Will be the addresses to integers
struct timeval start, end;
//------------------------------------------

int main(int argc, char *argv[]){
	
	gettimeofday(&start,NULL);
	
	pid_t pid;
	filePtrIn = fopen("P1Input.txt","r");
    if(filePtrIn == NULL) {
            printf("Error: File open failed.\n");
            exit(1);
    }

    filePtrOut = fopen("P1Part2.txt","w");
    if(filePtrOut == NULL) {
            printf("Error: File open failed.\n");
            exit(1);
    }

    L = atoi(argv[1]);
    H = atoi(argv[2]);
    PN = atoi(argv[3]);

    if(L < 3000) {
            fprintf(filePtrOut,"Error: Argument L should be greater than or equal to 3000.\n");
            exit(1);
    }
    if(H <= 0 || H > 10) {
            fprintf(filePtrOut,"Error: Argument H should be greater than 0 and less than or equal to 10.\n");
            exit(1);
    }
	
	
	//Initialize an array of integers, representation of our file
	fileArray = (int*) malloc(L*sizeof(int));
	
    for(int i=0; i<L; i++) {
        fscanf(filePtrIn,"%d",&fileArray[i]);
    }
	
	int offset = 0;
	int rec = L/(PN-1);
	int totalProc = 1;
	int procNum = 0;
	int isChild = 0;
	pipe(fd);
	pipe(cd);
	pipe(final);
	int parentPID;
	while(totalProc < PN){
	    totalProc++;
		pid = fork();
		parentPID = getppid();
		if(pid < 0){
			fprintf(filePtrOut,"Something went wrong and child process could not be created.\n");
			exit(1);
		}else if(pid == 0){
			isChild = 1;
			procNum = totalProc;
		}else{
			break;
		}
	}
	
	fprintf(filePtrOut,"Hi I'm process %d and my parent is %d.\n",getpid(),parentPID);
	
	if(isChild) {
	    wait(NULL);
	    childProcessCode(procNum);
	    exit(0);
	}
	
	//This allows the child processes to read the file and process the information
	for(int w=0; w < PN-1; w++){
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
	
	for(int r = 0; r < PN-1; r++){
		read(final[0], newpackage, 3*sizeof(float));
		if((int) newpackage[0] > finalmaxnum) {
		    finalmaxnum = (int) newpackage[0];
		}
		finalavg += newpackage[1]/newpackage[2];
	}
	
	fprintf(filePtrOut,"Max = %d, Avg = %f.\n\n",finalmaxnum,finalavg);
	for(int i=0;i<H;i++) {
	    fprintf(filePtrOut,"Hi I am process %d and I found the hidden key in position A[%d].\n",keyInfo[2*i],keyInfo[2*i+1]);
	}
	
	gettimeofday(&end,NULL);
	float runTime = (end.tv_usec - start.tv_usec)/1000000.0;

    printf("Time Elapsed: %f.\n",runTime);
	
	return 0;
}


//Returns the number of hidden keys found
int childProcessCode(int index){
    int isLast = 0;
    if(index == PN) {
        isLast = 1;
    }
	int offset;
	int n = read(fd[0], &offset, sizeof(int));
	int linestoRead;
	int procID = getpid();
	if(isLast) {
	    linestoRead = L-offset;
	}
	else {
	    linestoRead = L/(PN-1);
	}
	
	//Now we can get the total and max from array
	float weightAvg;
	int maxNum = 0;
	int hiddenkeys[H];
	int countKeys = 0;
	int indice = 0;
	for(int x = offset; x < offset+linestoRead; x++){
		//Hidden keys is going to hold the natural indicies of the negative integers
		if(fileArray[x] > maxNum){
			maxNum = fileArray[x];
		}
		if(fileArray[x] == -1){
			hiddenkeys[countKeys] = x;
			countKeys++;
            cpidnum[indice] = getpid();
			index++;
			write(cd[1],&procID,sizeof(int));
			write(cd[1],&x,sizeof(int));
		}
		weightAvg = weightAvg + ((float)fileArray[x]);
	}
	float * newpackage = (float*) malloc(3*sizeof(float));
	newpackage[0] = (float) maxNum;
	newpackage[1] = weightAvg;
	newpackage[2] = (float) linestoRead;
	write(final[1], newpackage, 3 * sizeof(float));
}
