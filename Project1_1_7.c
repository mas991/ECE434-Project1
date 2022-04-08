#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(){
	pid_t pid = fork();
	int i = 2;
	while(1){
		if(pid<0){
			printf("Error in Fork after %d processes were created\n", i);
			return;
		}
		else if (pid==0){
			while(1){
			}
		}
		else{
			printf("Forking Child Number %d\n", i);
			i++;
			pid = fork();
		}
	}
}
