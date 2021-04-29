#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static char File_Name[] = "./mmv.out";

void Fail()
{
	printf("fork fail\n");
    exit(-1);
}

void Child(int argc, char *argv[])
{
	if (argc == 1) execlp(File_Name, "", NULL);
	else execlp(File_Name, argv[1], argv[2], NULL);
	exit(0);
}

void Parant(pid_t pid)
{
	printf("Successful (#%d)!\n", pid);
    //wait(NULL);
}

int main(int argc, char *argv[])
{
	pid_t pid = fork();

	if (pid < 0) Fail();
    else if (pid == 0) Child(argc, argv);
    else Parant(pid);
	
	return 0;
}