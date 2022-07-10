#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int demo(char *uname);

int main()
{
	char (*c)[5];
	char t[5]="hi";
	c=&t;
	printf("%s\n",*c);
	printf("%-10s  %-15s\n","Title 1","Title 2");
	
	
	char s1[20]="Hello";
	char s2[10]="Hello";
	int ans=strcmp(s1,s2);
	printf("%d\n",ans);
	
	demo(s1);
	
	int tnum_db=open("DataBase/train_number.txt", O_RDWR | O_CREAT, S_IRWXU);
	perror("open");
	int pnr_db=open("DataBase/PNR.txt", O_RDWR | O_CREAT, S_IRWXU);
	perror("open");
	
	int n1,n2;
	read(tnum_db,&n1,sizeof(n1));
	perror("read");
	read(pnr_db,&n2,sizeof(n2));
	perror("write");
	
	printf("train number : %d\n",n1);
	printf("pnr number : %d\n",n2);
	
}

int demo(char *uname)
{
	char un[20];
	strcpy(un,uname);
	printf("%s\n",un);
	return 0;
}	
