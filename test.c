#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	while(0)
	{
		printf("Loading   ");
		fflush(stdout);
		printf("\b\b\b");
		fflush(stdout);
		printf(".");
		fflush(stdout);
		sleep(1);
		printf(".");
		fflush(stdout);
		sleep(1);
		printf(".");
		fflush(stdout);
		sleep(1);
		printf("\r");
		fflush(stdout);
	}
	char a[]={'|','/','_','\\'};
		printf("Loading  ");
		fflush(stdout);
		printf("\b");
		fflush(stdout);
	while(1)
	{
		for(int i=0;i<4;i++)
		{
			printf("%c    ",a[i]);
			fflush(stdout);
			printf("\b\b\b\b\b");
			fflush(stdout);
			sleep(1);
		}
	}
}
