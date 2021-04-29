#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
static char message[] = "\\----Say Hello to s1083321!----\\\n";

int main(int argc, char *argv[])
{
	int fRead, fWrite;	
	char tmp[100];

	// open files
	if (argc == 2)
	{
		fRead = open(argv[0], O_RDONLY);
		fWrite = open(argv[1], O_WRONLY|O_CREAT);
	}
	else
	{
		fRead = open("blake.txt", O_RDONLY);
		fWrite = open("happy.tmp", O_WRONLY|O_CREAT);
	}	

	// write message in target file
	write(fWrite, message, strlen(message));
	while(read(fRead, tmp, 1)) write(fWrite, tmp, strlen(tmp));

	close(fRead);
	close(fWrite);

	return 0;
}