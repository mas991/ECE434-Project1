CC = gcc

all: P1randList P1part1 P1part2 P1part3 Project1_1_7 P1Var

P1randList: P1randList.c
	$(CC) P1randList.c -o P1randList

P1part1: P1part1.c
	$(CC) P1part1.c -o P1part1

P1part2: P1part2.c
	$(CC) P1part2.c -o P1part2

P1part3: P1part3.c
	$(CC) P1part3.c -o P1part3

Project1_1_7: Project1_1_7.c
	$(CC) Project1_1_7.c -o Project1_1_7
	
P1Var: P1Var.c
	$(CC) P1Var.c -o P1Var

clean:
	rm -f P1randList P1part1 P1part2 P1part3 Project1_1_7 P1Var
