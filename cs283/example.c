#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<stdint.h>
#include "csapp.h"
sigjmp_buf buff;

void handler(int );

int main(int argv, char ** argc)
{
	signal (SIGINT, handler);
	int v=0;
	v = sigsetjmp(buff,1);
	int f1,f2,nbytes;
	char buf[100];
	if (v == 0)
	{
		printf("inhere: %s, %s", argc[1],argc[2]);
		if((f1 = open(argc[1],O_RDONLY)) <0) {
				perror("open");
				exit(1);
				}
		
		if((f2 = open(argc[2], O_CREAT | O_WRONLY,0644)) < 0) {
			perror("open");
			exit(1);
		}
		printf("Files OPENED");
		
		while((nbytes=read(f1,buf, sizeof(buf))) > 0)
			write(f2, buf, nbytes);
		close(f1);
		close(f2);
	}
	else if (v == 1)
	{
		close (f1);
		close (f2);
		unlink(argc[2]);
	}
}

void handler(int sig)
{
	siglongjmp(buff,1);
	printf("\nhandled\n");
	exit(0);
}


