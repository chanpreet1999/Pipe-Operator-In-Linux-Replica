#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

	
void error(char *errorMsg);

/*
-----------------------------
| fileno |  stream name   	|
|---------------------------|
|   0	 |	standard input	|
|   1	 |	standard output	|
|   2	 |	standaed error	|
-----------------------------

*/

int main(int argc, int argv[])
{
	//parent is for read and child is for write
	int fd[2];
	if( pipe(fd) == -1 )
	{
		error("Cannot create pipe operator");
	}

	pid_t pid = fork();
	if(pid == -1)
	{
		error("Cannot fork process");
	}

	if(!pid)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		if(execl("./hello2", "./hello2", NULL) == -1)
		{
			error("Cannot run process");
		}	
	}
	//assuming that this is parent
	//parent will read the data  and the child will write to it
	//so the parent's write stream should be closed
	dup2(fd[0], 0);
	close(fd[1]);

	char line[255];
	while(fgets( line, 255, stdin )) {
		printf( "Received data from child : %s", line );
		if(line[0] == '\0')
		{
			printf( "The content received is : %s", line );
		}	
	}

	return 0;
}


void error(char* msg)
{
	printf("%s\n", strerror(errno));
	printf("%s\n", msg);
	exit(1);
}