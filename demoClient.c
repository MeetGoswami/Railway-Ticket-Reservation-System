#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

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
	struct sockaddr_in serv;
	int sd;
	char buf[512];
	sd = socket(AF_UNIX,SOCK_STREAM,0);
	
	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port=htons(5555);
	
	
	connect(sd, (void *)(&serv), sizeof(serv));
	
	struct userDetails ud;
	printf("Name : ");
	scanf(" %[^\n]",ud.name);
	printf("Gender (M/F/O) : ");
	scanf(" %[^\n]",&ud.gender);
	printf("Age : ");
	scanf("%d",&ud.age);
	printf("Mobile Number : ");
	scanf(" %[^\n]",ud.mob);
	printf("Email : ");
	scanf(" %[^\n]",ud.email);
	printf("Enter Username : ");
	scanf(" %[^\n]", ud.uname);
	printf("Enter Password : ");
	scanf(" %[^\n]", ud.passwd);
	
	write(sd, &ud, sizeof(ud));
	
	sleep(5);
	struct userDetails u;
	
	int fd=open("demo.txt",O_RDWR);
	read(fd,&u,sizeof(u));
	printf("User ID : %s\n",ud.uname);
	printf("User Name : %s\n",ud.name);
	printf("Password : %s\n",ud.passwd);
	printf("Gender : %c\n",ud.gender);
	printf("Age : %d\n",ud.age);
	printf("mob : %s\n",ud.mob);
	printf("email : %s\n",ud.email);
	printf("utype : %d\n",ud.utype);
	
}
