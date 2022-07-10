#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

struct userDetails
{
	char uname[30];
	char passwd[30];
	char name[30];
	char gender;
	int age;
	char mob[11];
	char email[30];
	int utype;
	int login;
	int delete;
};

int main()
{
	struct sockaddr_in serv, cli;
	int sd, sz, nsd;
	char buf[512];
	sd = socket(AF_UNIX,SOCK_STREAM,0);
	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port=htons(5555);
	
	struct userDetails ud;
	
	
	bind(sd, (void *)(&serv),sizeof(serv));
	perror("bind");
	printf("Size of struct = %lu\n",sizeof(ud));
	listen(sd,5);
	sz=sizeof(cli);
	nsd = accept(sd, (void *)(&cli), &sz);
	
	int n=read(nsd, &ud, sizeof(ud));
	perror("read");
	printf("Data read=%d\n",n);
	
	printf("User ID : %s\n",ud.uname);
	printf("User Name : %s\n",ud.name);
	printf("Password : %s\n",ud.passwd);
	printf("Gender : %c\n",ud.gender);
	printf("Age : %d\n",ud.age);
	printf("mob : %s\n",ud.mob);
	printf("email : %s\n",ud.email);
	printf("utype : %d\n",ud.utype);
	
	int fd=open("demo.txt",O_RDWR);
	write(fd,&ud,sizeof(ud));
}
	
