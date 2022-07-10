#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

struct bookingDetails
{
	long pnr;
	char uname[30];
	char pname[50];
	int train_num;
	char train_name[50];
	char source[30];
	char dest[30];
	int booked_seats;
	int delete;
};

int main()
{
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);
	
	char *PNR_Details="DataBase/PNR.txt";
	int pnr_db=open(PNR_Details,O_RDWR);
	
	char *BookingDatabase="DataBase/BookingData.txt";
	int booking_db=open(BookingDatabase,O_RDWR);
	
	struct userDetails ud;
	
	while(read(user_db,&ud,sizeof(ud)))
	{
		printf("User ID : %s\n",ud.uname);
		printf("User Name : %s\n",ud.name);
		printf("Password : %s\n",ud.passwd);
		printf("Gender : %c\n",ud.gender);
		printf("Age : %d\n",ud.age);
		printf("mob : %s\n",ud.mob);
		printf("email : %s\n",ud.email);
		printf("utype : %d\n",ud.utype);
		printf("login : %d\n",ud.login);
		printf("delete : %d\n",ud.delete);
		printf("\n\n");
	}
	
	long pnr=-1;
	read(pnr_db,&pnr,sizeof(pnr));
	printf("Latest PNR : %ld\n",pnr);
	
	struct bookingDetails bd;
	while(read(booking_db,&bd,sizeof(bd)))
	{
		printf("PNR : %ld\n",bd.pnr);
		printf("User Name : %s\n",bd.uname);
		printf("Passenger : %s\n",bd.pname);
		printf("Train No. : %d\n",bd.train_num);
		printf("Train Name : %s\n",bd.train_name);
		printf("Source : %s\n",bd.source);
		printf("Dest : %s\n",bd.dest);
		printf("Delete : %d\n",bd.delete);
		printf("\n\n");
	}
	
	
}

	
