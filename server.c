#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

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

struct trainDetails
{
	int train_num;
	char train_name[50];
	char source[30];
	char dest[30];
	int available_seats;
	int total_seats;
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

int main_menu(int client);
int login(int client_sock);
int signup(int client_sock);
int customer_menu(int client_sock,int user_offset,char *username);
int logout(int client_sock,int user_offset);
int view_profile(int client_sock,int user_offset);
int update_profile(int client_sock,int user_offset);
int book_ticket(int client_sock,char *username);
int train_by_num(int client_sock,int tnum,char *username);
int train_by_location(int client_sock,char *username);
int previous_booking(int client_sock,char *username);
int update_booking(int client_sock,char *username);
int cancel_booking(int client_sock, char *username);
int admin_services(int client_sock,int offset);
int manage_users(int client_sock);
int modify_user(int client_sock);
int remove_user(int client_sock);
int manage_trains(int client_sock);
int add_train(int client_sock);
int modify_train(int client_sock);
int remove_train(int client_sock);


int main()
{
	struct sockaddr_in serv, cli;
	int sd, sz, nsd;
	sd = socket(AF_UNIX,SOCK_STREAM,0);
	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port=htons(8000);
	
	bind(sd, (void *)(&serv),sizeof(serv));
	perror("bind");
	
	listen(sd,5);
	perror("listen");
	sz=sizeof(cli);
	char end[]="end";
	int i=0;
	
	/*
	char *UserDatabase="DataBase/UserData.txt";
	char *TrainDatabase="DataBase/TrainData.txt";
	char *BookingDatabase="DataBase/BookingData.txt"
	
	int user_db=open(UserDatabase,O_RDWR);
	int train_db=open(TrainDatabase,O_RDWR);
	int booking_db=open(BookingDatabase,O_RDWR);
	*/
	
	
	while(1)
	{
		int client = accept(sd, (void *)(&cli), &sz);
		perror("accept");
		if(!fork())
		{
			close(sd);
			main_menu(client);
			break;
		}
		else
		{
			close(nsd);
		}
	}
	return 0;
}



int main_menu(int client)
{
	while(1)
	{
		int choice;
		read(client, &choice, sizeof(choice));
		if(choice==1)
		{
			login(client);
		}
		else if(choice==2)
		{
			signup(client);
		}
		else if(choice==3)
		{
			close(client);
			return 0;
		}
		else
		{
			//Invalid input by user. Thus ignore it.
		}
	}
	return 0;
}

int login(int client_sock)
{
	printf("\nIN LOGIN()\n");
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);
	int choice=-1;
	int valid=0;
	int offset=0;
	struct userDetails user;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
		printf("choice received\n");
		if(choice==0)
		{
			close(user_db);
			return 0;
		}
		else  if(choice==1 || choice==2 || choice==3)
		{
			
			char uname[30];
			char passwd[30];
			
			read(client_sock,uname,sizeof(uname));
			printf("uname received %s\n",uname);
			read(client_sock,passwd,sizeof(passwd));
			printf("passwd received %s\n",passwd);
			struct flock lock;
			lock.l_type=F_WRLCK;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_whence = SEEK_SET;
			lock.l_pid = getpid();
			printf("waiting for unclock\n");
			fcntl(user_db,F_SETLKW, &lock);
			printf("locked\n");
			
			valid=0;
			printf("Comparing\n");
			while(read(user_db,&user,sizeof(user)))
			{
				if(strcmp(uname,user.uname)==0 && strcmp(passwd,user.passwd)==0 && choice==user.utype && user.delete==0)
				{
					if((user.utype==1 || user.utype==3) && user.login==0)
					{
						
						user.login=1;
						lseek(user_db,-1*sizeof(user),SEEK_CUR);
						write(user_db,&user,sizeof(user));
						valid=1;
						break;
					}
					else if((user.utype==1 || user.utype==3) && user.login==1)
					{
						valid=2;
						break;
					}
					else if(user.utype==2)
					{
						valid=1;
						break;
					}
					else
					{
					}
				}
				offset++;
			}
			printf("Comparing over\n");
			lock.l_type=F_UNLCK;
			fcntl(user_db,F_SETLKW, &lock);
			printf("Unlocked\n");
			write(client_sock,&valid,sizeof(valid));
			printf("status %d sent to client\n",valid);
			if(valid==1)
			{
				break;
			}
		}
	}
	
	close(user_db);
	
	if(valid==1 && choice==3)
	{
		admin_services(client_sock,offset);
	}
	else if(valid==1 && (choice==1 || choice==2))
	{
		customer_menu(client_sock,offset,user.uname);
	}
	else
	{
	}
	
	return 0;
}


int signup(int client_sock)
{
	printf("IN SIGNUP()\n");
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);
	int choice=-1;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
		printf("choice received\n");
		if(choice==0)
		{
			close(user_db);
			return 0;
		}
		else if(choice==3)
		{
				char admin_key[20]="admin123";
				char received_key[20];
				int valid=0;
				read(client_sock,received_key,sizeof(received_key));
				printf("key received\n");
				if(strcmp(received_key,admin_key)==0)
				{
					valid=1;
				}
				write(client_sock,&valid,sizeof(valid));
				printf("key status sent\n");
				if(valid==1)
				{
					int unameValid=0;
					struct flock lock;
					struct userDetails ud;
					read(client_sock,&ud,sizeof(ud));
					printf("user details received\n");
					printf("User ID : %s\n",ud.uname);
					printf("User Name : %s\n",ud.name);
					printf("Password : %s\n",ud.passwd);
					printf("Gender : %c\n",ud.gender);
					printf("Age : %d\n",ud.age);
					printf("mob : %s\n",ud.mob);
					printf("email : %s\n",ud.email);
					printf("utype : %d\n",ud.utype);
					
					
					struct userDetails old;	
						unameValid=1;
						lock.l_type=F_RDLCK;
						lock.l_start = 0;
						lock.l_len = 0;
						lock.l_whence = SEEK_SET;
						lock.l_pid = getpid();
						printf("lock waiting\n");
						fcntl(user_db,F_SETLKW, &lock);
						printf("locked\n");
						printf("comparison starts\n");
						while(read(user_db,&old,sizeof(old)))
						{
							if(strcmp(ud.uname,old.uname)==0 && old.delete==0)
							{
								unameValid=0;
								break;
							}
						}
						printf("comparison ends\n");
						lock.l_type=F_UNLCK;
						fcntl(user_db,F_SETLKW, &lock);
						printf("unlocked\n");
						
						if(unameValid==1)
						{
							lock.l_start = 0;
							lock.l_len = 0;
							lock.l_whence = SEEK_SET;
							lock.l_pid = getpid();
							printf("waiting lock\n");
							fcntl(user_db,F_SETLKW, &lock);
							printf("locked\n");
							lseek(user_db,0,SEEK_END);
							write(user_db,&ud,sizeof(ud));
							printf("written in file\n");
							lock.l_type=F_UNLCK;
							fcntl(user_db,F_SETLKW, &lock);
							printf("unlocked\n");
						}
						write(client_sock,&unameValid,sizeof(unameValid));
						printf("status send to client\n");
					}
				}
				else if(choice==2 || choice ==1)
				{
					int unameValid=0;
					struct flock lock;
					struct userDetails ud;
					read(client_sock,&ud,sizeof(ud));
					printf("user details received\n");
					printf("User ID : %s\n",ud.uname);
					printf("User Name : %s\n",ud.name);
					printf("Password : %s\n",ud.passwd);
					printf("Gender : %c\n",ud.gender);
					printf("Age : %d\n",ud.age);
					printf("mob : %s\n",ud.mob);
					printf("email : %s\n",ud.email);
					printf("utype : %d\n",ud.utype);
					
					
					struct userDetails old;	
						unameValid=1;
						lock.l_type=F_RDLCK;
						lock.l_start = 0;
						lock.l_len = 0;
						lock.l_whence = SEEK_SET;
						lock.l_pid = getpid();
						printf("lock waiting\n");
						fcntl(user_db,F_SETLKW, &lock);
						printf("locked\n");
						printf("comparison starts\n");
						while(read(user_db,&old,sizeof(old)))
						{
							if(strcmp(ud.uname,old.uname)==0 && old.delete==0)
							{
								unameValid=0;
								break;
							}
						}
						printf("comparison ends\n");
						lock.l_type=F_UNLCK;
						fcntl(user_db,F_SETLKW, &lock);
						printf("unlocked\n");
						write(client_sock,&unameValid,sizeof(unameValid));
						printf("status send to client\n");
						if(unameValid==1)
						{
							lock.l_start = 0;
							lock.l_len = 0;
							lock.l_whence = SEEK_SET;
							lock.l_pid = getpid();
							printf("waiting lock\n");
							fcntl(user_db,F_SETLKW, &lock);
							printf("locked\n");
							lseek(user_db,0,SEEK_END);
							write(user_db,&ud,sizeof(ud));
							printf("written in file\n");
							lock.l_type=F_UNLCK;
							fcntl(user_db,F_SETLKW, &lock);
							printf("unlocked\n");
						}
					}
				else
				{
				}
					
					
					break;
				}
	close(user_db);
	return 0;
}

int customer_menu(int client_sock,int user_offset,char *username)
{
	printf("IN CUSTOMER_MENU()\n");
	int choice=-1;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
		
		//LOGOUT
		if(choice==0)
		{
			logout(client_sock,user_offset);
			break;
		}
		
		
		//VIEW PROFILE
		else if(choice==1)
		{
			view_profile(client_sock,user_offset);
		}
		
		//UPDATE PROFILE
		else if(choice==2)
		{
			update_profile(client_sock,user_offset);
		}
		
		//BOOK TICKET
		else if(choice==3)
		{
			book_ticket(client_sock,username);
		}
		
		//VIEW PRIVIOUS BOOKING
		else if(choice==4)
		{
			previous_booking(client_sock,username);
		}
		
		//UPDATE BOOKING
		else if(choice==5)
		{
			update_booking(client_sock,username);
		}
		
		//CANCEL BOOKING
		else if(choice==6)
		{
			cancel_booking(client_sock,username);
		}
		else
		{
			//INVALID INPUT. NO OPERATION
		}
	}
return 0;
}


int logout(int client_sock,int user_offset)
{
	printf("IN LOGOUT()\n");
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);

	struct userDetails udt;
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_start = user_offset*sizeof(udt);
	lock.l_len = sizeof(udt);
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	
	fcntl(user_db,F_SETLKW, &lock);
			
	lseek(user_db,user_offset*sizeof(udt),SEEK_SET);
	read(user_db,&udt,sizeof(udt));
	udt.login=0;
	lseek(user_db,user_offset*sizeof(udt),SEEK_SET);
	write(user_db,&udt,sizeof(udt));
			
	lock.l_type=F_UNLCK;
	fcntl(user_db,F_SETLKW, &lock);
	
	close(user_db);
	return 0;
}

int view_profile(int client_sock,int user_offset)
{
	printf("IN VIEW_PROFILE()\n");
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);	
	
	struct userDetails udt;
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start = user_offset*sizeof(udt);
	lock.l_len = sizeof(udt);
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
			
	fcntl(user_db,F_SETLKW, &lock);
	
	lseek(user_db,user_offset*sizeof(udt),SEEK_SET);
	read(user_db,&udt,sizeof(udt));
	write(client_sock,&udt,sizeof(udt));
			
	lock.l_type=F_UNLCK;
	fcntl(user_db,F_SETLKW, &lock);
	
	close(user_db);
	return 0;
}

int update_profile(int client_sock,int user_offset)
{
	printf("IN UPDATE_PROFILE()\n");
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);	
	
	struct userDetails udt;
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start = user_offset*sizeof(udt);
	lock.l_len = sizeof(udt);
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
			
	fcntl(user_db,F_SETLKW, &lock);
	
	lseek(user_db,user_offset*sizeof(udt),SEEK_SET);
	read(user_db,&udt,sizeof(udt));
	write(client_sock,&udt,sizeof(udt));
			
	lock.l_type=F_UNLCK;
	fcntl(user_db,F_SETLKW, &lock);
	
	int field_num=-1;
	read(client_sock,&field_num,sizeof(field_num));
	
	if(field_num==0)
	{
		close(user_db);
		return 0;
	}
	
	else if(field_num==1)
	{
		//CANNOT CHANGE USERNAME
	}
	else if(field_num>=2 && field_num<=7)
	{
		read(client_sock,&udt,sizeof(udt));
		lock.l_type=F_WRLCK;
		
		fcntl(user_db,F_SETLKW, &lock);
		lseek(user_db,user_offset*sizeof(udt),SEEK_SET);
		
		write(user_db,&udt,sizeof(udt));
			
		lock.l_type=F_UNLCK;
		fcntl(user_db,F_SETLKW, &lock);
	}
	else
	{
		//INVALID INPUT BY USER. NO OPERATION.
	}
	close(user_db);
	return 0;	
}

int book_ticket(int client_sock,char *username)
{
	printf("IN BOOK_TICKET()\n");
	int choice=-1;
	
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
		if(choice==0)
		{
			return 0;
		}
		else if(choice==1)
		{
			int train_num=-1;
			read(client_sock,&train_num,sizeof(train_num));
			train_by_num(client_sock,train_num,username);	
		}
		else if(choice==2)
		{
			train_by_location(client_sock,username);
		}
		else
		{
		}
	}
		
return 0;	
}

int train_by_num(int client_sock,int tnum,char *username)
{
	printf("IN TRAIN_BY_NUM()\n");
	char *TrainDatabase="DataBase/TrainData.txt";
	int train_db=open(TrainDatabase,O_RDWR);
	
	struct trainDetails td;
	int status=0;
	
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start = (tnum-1)*sizeof(td);
	lock.l_len = sizeof(td);
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
			
	fcntl(train_db,F_SETLKW, &lock);
	printf("TrainDB locked\n");
	lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
	read(train_db,&td,sizeof(td));
	lock.l_type=F_UNLCK;
	fcntl(train_db,F_SETLKW, &lock);
	printf("TrainDB unlocked\n");
	if(td.train_num==tnum && td.delete==0)
	{
		status=1;
	}
	write(client_sock,&status,sizeof(status));
	printf("status sent\n");
	if(status==1)
	{
		write(client_sock,&td,sizeof(td));
		printf("train details sent\n");
		int choice=-1;
		read(client_sock,&choice,sizeof(choice));
		printf("choice received\n");
		if(choice==1)
		{
			printf("choice 1\n");
			char passenger_name[50];
			int seats=-1;
			int booking_status=-1;
			read(client_sock,passenger_name,sizeof(passenger_name));
			printf("passenger name recived\n");
			read(client_sock,&seats,sizeof(seats));
			printf("no. of seats received\n");
			lock.l_type=F_WRLCK;
			fcntl(train_db,F_SETLKW, &lock);
			printf("TrainDB locked\n");
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			read(train_db,&td,sizeof(td));
			
			if(td.available_seats>=seats && td.delete==0)
			{
				printf("Seats available\n");
				td.available_seats-=seats;
				lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
				write(train_db,&td,sizeof(td));
				printf("seats reduced in train\n");
				lock.l_type=F_UNLCK;
				fcntl(train_db,F_SETLKW, &lock);
				printf("TrainDB unlocked\n");
				
				char *PNR_Details="DataBase/PNR.txt";
				int pnr_db=open(PNR_Details,O_RDWR);
				char *BookingDatabase="DataBase/BookingData.txt";
				int booking_db=open(BookingDatabase,O_RDWR);
				
				long new_pnr=-1;
				lock.l_type=F_WRLCK;
				lock.l_start = 0;
				lock.l_len = 0;
				lock.l_whence = SEEK_SET;
				fcntl(pnr_db,F_SETLKW, &lock);
				printf("PNR DB locked\n");
				read(pnr_db,&new_pnr,sizeof(new_pnr));
				new_pnr+=1;
				lseek(pnr_db,0,SEEK_SET);
				write(pnr_db,&new_pnr,sizeof(new_pnr));
				lock.l_type=F_UNLCK;
				fcntl(pnr_db,F_SETLKW, &lock);
				printf("PNR DB unlocked\n");
				
				struct bookingDetails bd;
				bd.pnr=new_pnr;
				strcpy(bd.uname,username);
				strcpy(bd.pname,passenger_name);
				bd.train_num=td.train_num;
				strcpy(bd.train_name,td.train_name);
				strcpy(bd.source,td.source);
				strcpy(bd.dest,td.dest);
				bd.booked_seats=seats;
				bd.delete=0;
				
				printf("booking details created\n");
				
				lock.l_type=F_WRLCK;
				fcntl(booking_db,F_SETLKW, &lock);
				printf("BookingDB locked\n");
				lseek(booking_db,0,SEEK_END);
				write(booking_db,&bd,sizeof(bd));
				printf("booking details written in file\n");
				lock.l_type=F_UNLCK;
				fcntl(booking_db,F_SETLKW, &lock);				
				printf("BookingDB unlocked\n");
				booking_status=0;
				close(booking_db);
				close(pnr_db);
				write(client_sock,&booking_status,sizeof(booking_status));
				printf("booking status sent\n");
				write(client_sock,&bd,sizeof(bd));
				printf("booking details sent\n");
			}
			else
			{
				printf("Seats not available\n");
				booking_status=1;
				write(client_sock,&booking_status,sizeof(booking_status));
				printf("booking status sent\n");
			}
			
			
		}
		else
		{
			printf("choice=2\n");
			//NO OPERATION
		}
	}
	close(train_db);
	return 0;
}

int train_by_location(int client_sock,char *username)
{
	printf("IN TRAIN_BY_LOC()\n");
	char *TrainDatabase="DataBase/TrainData.txt";
	int train_db=open(TrainDatabase,O_RDWR);
	
	char source[30],dest[30];
	read(client_sock,&source,sizeof(source));
	read(client_sock,&dest,sizeof(dest));
	
	struct trainDetails td;
	int status=0;
	
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
			
	fcntl(train_db,F_SETLKW, &lock);
		
	int count=0;
	
	while(read(train_db,&td,sizeof(td)))
	{
		if(strcmp(source,td.source)==0 && strcmp(dest,td.dest)==0 && td.delete==0)
		{
			count++;
		}
	}
	
	lock.l_type=F_UNLCK;
	fcntl(train_db,F_SETLKW, &lock);
	
	write(client_sock,&count,sizeof(count));
	if(count==0)
	{
		//NO TRAINS FOR GIVEN LOCATIONS
		close(train_db);
		return 0;
	}
	else
	{
		lock.l_type=F_WRLCK;
		fcntl(train_db,F_SETLKW, &lock);
		lseek(train_db,0,SEEK_SET);
		while(read(train_db,&td,sizeof(td)))
		{
			if(strcmp(source,td.source)==0 && strcmp(dest,td.dest)==0 && td.delete==0)
			{
				write(client_sock,&td,sizeof(td));
			}
		}
	
		int choice=-1;
		read(client_sock,&choice,sizeof(choice));
		if(choice>0)
		{
			int tnum=choice;
			char passenger_name[50];
			int seats=-1;
			int booking_status=-1;
			read(client_sock,passenger_name,sizeof(passenger_name));
			read(client_sock,&seats,sizeof(seats));
			
			lock.l_type=F_WRLCK;
			fcntl(train_db,F_SETLKW, &lock);
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			read(train_db,&td,sizeof(td));
			
			if(td.available_seats>=seats && td.delete==0)
			{
				td.available_seats-=seats;
				lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
				write(train_db,&td,sizeof(td));
				lock.l_type=F_UNLCK;
				fcntl(train_db,F_SETLKW, &lock);
				
				char *PNR_Details="DataBase/PNR.txt";
				int pnr_db=open(PNR_Details,O_RDWR);
				char *BookingDatabase="DataBase/BookingData.txt";
				int booking_db=open(BookingDatabase,O_RDWR);
				
				long new_pnr=-1;
				lock.l_type=F_WRLCK;
				lock.l_start = 0;
				lock.l_len = 0;
				lock.l_whence = SEEK_SET;
				fcntl(pnr_db,F_SETLKW, &lock);
				read(pnr_db,&new_pnr,sizeof(new_pnr));
				new_pnr+=1;
				lseek(pnr_db,0,SEEK_SET);
				write(pnr_db,&new_pnr,sizeof(new_pnr));
				lock.l_type=F_UNLCK;
				fcntl(pnr_db,F_SETLKW, &lock);
				
				struct bookingDetails bd;
				bd.pnr=new_pnr;
				strcpy(bd.uname,username);
				strcpy(bd.pname,passenger_name);
				bd.train_num=td.train_num;
				strcpy(bd.source,td.source);
				strcpy(bd.dest,td.dest);
				bd.booked_seats=seats;
				bd.delete=0;
				
				lock.l_type=F_WRLCK;
				fcntl(booking_db,F_SETLKW, &lock);
				lseek(booking_db,0,SEEK_END);
				write(booking_db,&bd,sizeof(bd));
				
				lock.l_type=F_UNLCK;
				fcntl(booking_db,F_SETLKW, &lock);				
				
				booking_status=0;
				close(booking_db);
				close(pnr_db);
			}
			else
			{
				booking_status=1;
			}
			write(client_sock,&booking_status,sizeof(booking_status));
		}
		else
		{
			//NO OPERATION
		}
	}
	close(train_db);
	return 0;
}

int previous_booking(int client_sock,char *username)
{
	printf("IN PREV_BOOKING()\n");
	char *BookingDatabase="DataBase/BookingData.txt";
	int booking_db=open(BookingDatabase,O_RDWR);
	printf("Username : %s\n",username);
	int count=0;
	
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	
	fcntl(booking_db,F_SETLKW, &lock);
	struct bookingDetails bd;
	
	while(read(booking_db,&bd,sizeof(bd)))
	{
		if(strcmp(bd.uname,username)==0 && bd.delete==0)
		{
			count++;
		}
	}
	
	write(client_sock,&count,sizeof(count));
	printf("count = %d\n",count);
	if(count>0)
	{
		lseek(booking_db,0,SEEK_SET);
		while(read(booking_db,&bd,sizeof(bd)))
		{
			if(strcmp(bd.uname,username)==0 && bd.delete==0)
			{
				write(client_sock,&bd,sizeof(bd));
			}
		}
	}
	lock.l_type=F_UNLCK;
	fcntl(booking_db,F_SETLKW, &lock);
	close(booking_db);

	return 0;
}

int update_booking(int client_sock,char *username)
{
	printf("IN UPDATE_BOOKING()\n");
	long input_pnr=-1;
	read(client_sock,&input_pnr,sizeof(input_pnr));
	printf("Reeived PNR : %ld\n",input_pnr);
	char *BookingDatabase="DataBase/BookingData.txt";
	int booking_db=open(BookingDatabase,O_RDWR);
	
	int valid=-1;
	
	if(input_pnr<0)
	{
		write(client_sock,&valid,sizeof(valid));
		printf("1.valid = %d sent\n",valid);
	}
	else
	{
		struct bookingDetails bd;
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_start = -1*sizeof(bd);
		lock.l_len = sizeof(bd);
		lock.l_whence = SEEK_END;
		lock.l_pid = getpid();
		
		fcntl(booking_db,F_SETLKW, &lock);
		read(booking_db,&bd,sizeof(bd));
		
		lock.l_type=F_UNLCK;
		fcntl(booking_db,F_SETLKW, &lock);
		if(bd.pnr<input_pnr)
		{
			write(client_sock,&valid,sizeof(valid));
			printf("2.valid = %d sent\n",valid);
		}
		else
		{
			
			
			lock.l_type=F_RDLCK;
			lock.l_start = (input_pnr-1)*sizeof(bd);
			lock.l_len = sizeof(bd);
			lock.l_whence = SEEK_SET;
			
			fcntl(booking_db,F_SETLKW, &lock);
			lseek(booking_db,(input_pnr-1)*sizeof(bd),SEEK_SET);
			read(booking_db,&bd,sizeof(bd));
			
			lock.l_type=F_UNLCK;
			fcntl(booking_db,F_SETLKW, &lock);
			if(strcmp(username,bd.uname)==0)
			{
				valid=0;
				write(client_sock,&valid,sizeof(valid));
				printf("3.valid = %d sent\n",valid);
				write(client_sock,&bd,sizeof(bd));
				printf("Booking details sent\n");
				int update_choice=-1;
				read(client_sock,&update_choice,sizeof(update_choice));
				printf("received choice = %d\n",update_choice);
				if(update_choice==1)
				{
					read(client_sock,&bd,sizeof(bd));
					lock.l_type=F_WRLCK;
					fcntl(booking_db,F_SETLKW, &lock);
					lseek(booking_db,(input_pnr-1)*sizeof(bd),SEEK_SET);
					write(booking_db,&bd,sizeof(bd));
					
					lock.l_type=F_UNLCK;
					fcntl(booking_db,F_SETLKW, &lock);
				}
				else if(update_choice==2)
				{
					int new_seats=-1;
					int update_status=-1;
					read(client_sock,&new_seats,sizeof(new_seats));
					
					char *TrainDatabase="DataBase/TrainData.txt";
					int train_db=open(TrainDatabase,O_RDWR);
					
					struct trainDetails td;
					lock.l_type=F_WRLCK;
					lock.l_start = (bd.train_num-1)*sizeof(td);
					lock.l_len = sizeof(td);
					lock.l_whence = SEEK_SET;
					
					fcntl(train_db,F_SETLKW, &lock);
					lseek(train_db,(bd.train_num-1)*sizeof(td),SEEK_SET);
					read(train_db,&td,sizeof(td));
					
					int diff=new_seats-bd.booked_seats;
					
					if(diff>0 && diff<=td.available_seats)
					{
						td.available_seats-=diff;
						lseek(train_db,(bd.train_num-1)*sizeof(td),SEEK_SET);
						write(train_db,&td,sizeof(td));
						lock.l_type=F_UNLCK;
						fcntl(train_db,F_SETLKW, &lock);
						
						lock.l_type=F_WRLCK;
						lock.l_start = (input_pnr-1)*sizeof(bd);
						lock.l_len = sizeof(bd);
						lock.l_whence = SEEK_SET;
						bd.booked_seats=new_seats;
						fcntl(booking_db,F_SETLKW, &lock);
						lseek(booking_db,(input_pnr-1)*sizeof(bd),SEEK_SET);
						write(booking_db,&bd,sizeof(bd));
					
						lock.l_type=F_UNLCK;
						fcntl(booking_db,F_SETLKW, &lock);
						update_status=0;
						
					}
					else if(diff<0)
					{
						td.available_seats-=diff;
						lseek(train_db,(bd.train_num-1)*sizeof(td),SEEK_SET);
						write(train_db,&td,sizeof(td));
						lock.l_type=F_UNLCK;
						fcntl(train_db,F_SETLKW, &lock);
						
						lock.l_type=F_WRLCK;
						lock.l_start = (input_pnr-1)*sizeof(bd);
						lock.l_len = sizeof(bd);
						lock.l_whence = SEEK_SET;
						bd.booked_seats=new_seats;
						fcntl(booking_db,F_SETLKW, &lock);
						lseek(booking_db,(input_pnr-1)*sizeof(bd),SEEK_SET);
						write(booking_db,&bd,sizeof(bd));
					
						lock.l_type=F_UNLCK;
						fcntl(booking_db,F_SETLKW, &lock);
						update_status=0;
						
					}
					else
					{
						//SEATS CANNOT BE UPDATED
					}
					write(client_sock,&update_status,sizeof(update_status));
					printf("update status %d sent\n",update_status);
					close(train_db);
				}
				else
				{
					//NO OPERATION
				}
			}
			else
			{
				write(client_sock,&valid,sizeof(valid));
				printf("4.valid = %d sent\n",valid);
			}
		
		}
	}
	close(booking_db);
	return 0;
}
				
int cancel_booking(int client_sock, char *username)
{
	printf("IN CANCEL_BOOKING()\n");
	long input_pnr=-1;
	read(client_sock,&input_pnr,sizeof(input_pnr));
	
	char *BookingDatabase="DataBase/BookingData.txt";
	int booking_db=open(BookingDatabase,O_RDWR);
	
	int valid=-1;
	
	if(input_pnr<0)
	{
		write(client_sock,&valid,sizeof(valid));
	}
	else
	{
		struct bookingDetails bd;
		struct flock lock;				
		lock.l_type=F_WRLCK;
		lock.l_start = (input_pnr-1)*sizeof(bd);
		lock.l_len = sizeof(bd);
		lock.l_whence = SEEK_SET;
		lock.l_pid=getpid();
		
		fcntl(booking_db,F_SETLKW, &lock);
		lseek(booking_db,(input_pnr-1)*sizeof(bd),SEEK_SET);
		read(booking_db,&bd,sizeof(bd));
		if(bd.pnr==input_pnr && strcmp(bd.uname,username)==0 && bd.delete==0)
		{
			bd.delete=1;
			lseek(booking_db,(input_pnr	-1)*sizeof(bd),SEEK_SET);
			write(booking_db,&bd,sizeof(bd));
			lock.l_type=F_UNLCK;
			fcntl(booking_db,F_SETLKW, &lock);
			
			char *TrainDatabase="DataBase/TrainData.txt";
			int train_db=open(TrainDatabase,O_RDWR);
			
			struct trainDetails td;
			int tnum=bd.train_num;
			lock.l_type=F_WRLCK;
			lock.l_start = (tnum-1)*sizeof(td);
			lock.l_len = sizeof(td);
			lock.l_whence = SEEK_SET;
			fcntl(train_db,F_SETLKW, &lock);
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			read(train_db,&td,sizeof(td));
			td.available_seats+=1;
			write(train_db,&td,sizeof(td));
			
			valid=0;
			write(client_sock,&valid,sizeof(valid));
		}
		else
		{
			write(client_sock,&valid,sizeof(valid));
		}
	}
	close(booking_db);
return 0;
}
			
//----------------------------------------------------------ADMIN SERVICES-----------------------------------------------------------------------------------------------

int admin_services(int client_sock,int offset)
{
	printf("IN ADMIN SERVICES()\n");
	int choice=-1;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
	
		if(choice==0)
		{
			logout(client_sock,offset);
			return 0;
		}
		else if(choice==1)
		{
			manage_trains(client_sock);
		}
		else if(choice==2)
		{
			manage_users(client_sock);
		}
		else
		{
			//NO OPERATION
		}
	}
	return 0;
}	

int manage_users(int client_sock)
{
	printf("IN MANAGE USERS()\n");
	int choice=-1;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
	
		if(choice==0)
		{
			return 0;
		}
		else if(choice==1)
		{
			signup(client_sock);
		}
		else if(choice==2)
		{
			modify_user(client_sock);
		}
		else if(choice==3)
		{
			remove_user(client_sock);
		}
		else
		{
			//NO OPERATION
		}
	}
	return 0;
}	


int modify_user(int client_sock)
{
	printf("IN MODIFY USERS()\n");
	char username[30];
	int valid=-1;
	read(client_sock,username,sizeof(username));
	
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);
	
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();
	
	fcntl(user_db,F_SETLKW,&lock);
	struct userDetails ud;
	int count=0;
	while(read(user_db,&ud,sizeof(ud)))
	{
		if(strcmp(ud.uname,username)==0 && ud.delete==0)
		{
			valid=0;
			break;
		}
		count++;
	}
	lock.l_type=F_UNLCK;
	fcntl(user_db,F_SETLKW,&lock);
	write(client_sock,&valid,sizeof(valid));
	if(valid==0)
	{
		update_profile(client_sock,count);
	}
	close(user_db);
	return 0;
}
	

int remove_user(int client_sock)
{
	printf("IN REMOVE USERS()\n");
	char username[30];
	int valid=-1;
	read(client_sock,username,sizeof(username));
	
	char *UserDatabase="DataBase/UserData.txt";
	int user_db=open(UserDatabase,O_RDWR);
	
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();
	
	fcntl(user_db,F_SETLKW,&lock);
	struct userDetails ud;
	int count=0;
	while(read(user_db,&ud,sizeof(ud)))
	{
		if(strcmp(ud.uname,username)==0 && ud.delete==0)
		{
			valid=0;
			break;
		}
		count++;
	}
	lock.l_type=F_UNLCK;
	fcntl(user_db,F_SETLKW,&lock);
	if(valid==0)
	{
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=count*sizeof(ud);
		lock.l_len=sizeof(ud);
		fcntl(user_db,F_SETLKW,&lock);
		lseek(user_db,count*sizeof(ud),SEEK_SET);
		read(user_db,&ud,sizeof(ud));
		ud.delete=1;
		lseek(user_db,count*sizeof(ud),SEEK_SET);
		write(user_db,&ud,sizeof(ud));
	}
	write(client_sock,&valid,sizeof(valid));
	
	close(user_db);
	return 0;
}


int manage_trains(int client_sock)
{
	printf("IN MANAGE TRAINS()\n");
	int choice=-1;
	while(choice!=0)
	{
		read(client_sock,&choice,sizeof(choice));
	
		if(choice==0)
		{
			return 0;
		}
		else if(choice==1)
		{
			add_train(client_sock);
		}
		else if(choice==2)
		{
			modify_train(client_sock);
		}
		else if(choice==3)
		{
			remove_train(client_sock);
		}
		else
		{
			//NO OPERATION
		}
	}
	return 0;
}			



			
int add_train(int client_sock)
{
	printf("IN ADD TRAINS()\n");
	char *trainNum="DataBase/train_number.txt";
	int tnum_db=open(trainNum,O_RDWR);
	int last_tnum=0;
	struct trainDetails td;
	read(client_sock,&td,sizeof(td));
	
	td.available_seats=td.total_seats;
	td.delete=0;
	
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();
	
	fcntl(tnum_db,F_SETLKW,&lock);
	read(tnum_db,&last_tnum,sizeof(last_tnum));
	last_tnum+=1;
	lseek(tnum_db,0,SEEK_SET);
	write(tnum_db,&last_tnum,sizeof(last_tnum));
	
	lock.l_type=F_UNLCK;
	fcntl(tnum_db,F_SETLKW,&lock);
	
	td.train_num=last_tnum;
	
	char *TrainDatabase="DataBase/TrainData.txt";
	int train_db=open(TrainDatabase,O_RDWR);
	
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	
	fcntl(train_db,F_SETLKW,&lock);
	lseek(train_db,0,SEEK_END);
	write(train_db,&td,sizeof(td));
	
	lock.l_type=F_UNLCK;
	fcntl(train_db,F_SETLKW,&lock);
	
	write(client_sock,&last_tnum,sizeof(last_tnum));
	return 0;
}

int modify_train(int client_sock)
{
	printf("IN MODIFY TRAINS()\n");
	int tnum=-1;
	read(client_sock,&tnum,sizeof(tnum));
	int valid=-1;
	if(tnum<0)
	{
		write(client_sock,&valid,sizeof(valid));
	}
	else
	{
		struct trainDetails td;
		char *TrainDatabase="DataBase/TrainData.txt";
		int train_db=open(TrainDatabase,O_RDWR);
		
		int offset=lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
		if(offset<0)
		{
			write(client_sock,&valid,sizeof(valid));
		}
		else
		{
			valid=0;
			write(client_sock,&valid,sizeof(valid));
			
			struct flock lock;
			lock.l_type=F_RDLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(tnum-1)*sizeof(td);
			lock.l_len=sizeof(td);
			lock.l_pid=getpid();
			fcntl(train_db,F_SETLKW,&lock);
			
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			read(train_db,&td,sizeof(td));
			lock.l_type=F_UNLCK;
			fcntl(train_db,F_SETLKW,&lock);
			write(client_sock,&td,sizeof(td));
			
			int fnum=-1;
			read(client_sock,&fnum,sizeof(fnum));
			
			if(fnum==1)
			{
				//TRAIN NUMBER CANNOT BE CHANGED
			}
			
			else if(fnum==2 || fnum==3 || fnum==4)
			{
				read(client_sock,&td,sizeof(td));
				lock.l_type=F_WRLCK;
				fcntl(train_db,F_SETLKW,&lock);
				lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
				write(train_db,&td,sizeof(td));
				lock.l_type=F_UNLCK;
				fcntl(train_db,F_SETLKW,&lock);
				int ack=1;
				write(client_sock,&ack,sizeof(ack));
			}
			else if(fnum==5 || fnum==6)
			{
				int seat_opt=-1;
				read(client_sock,&seat_opt,sizeof(seat_opt));
				if(seat_opt==1 || seat_opt==2)
				{
					int num=-1;
					read(client_sock,&num,sizeof(num));
					if(num>0)
					{
						read(client_sock,&td,sizeof(td));
						lock.l_type=F_WRLCK;
						fcntl(train_db,F_SETLKW,&lock);
						lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
						write(train_db,&td,sizeof(td));
						lock.l_type=F_UNLCK;
						fcntl(train_db,F_SETLKW,&lock);
						int ack=1;
						write(client_sock,&ack,sizeof(ack));
					}
					else
					{
						//INVALID INPUT BY USER
					}
				}
				else
				{
					//INVALID INPUT BY USER
				}
			}
			else
			{
				//INVALID INPUT BY USER
			}
		}
		close(train_db);
	}
	
	return 0;
}
					
int remove_train(int client_sock)
{
	printf("IN REMOVE TRAINS()\n");
	int tnum=-1;
	read(client_sock,&tnum,sizeof(tnum));
	int valid=-1;
	if(tnum<0)
	{
		write(client_sock,&valid,sizeof(valid));
	}
	else
	{
		struct trainDetails td;
		char *TrainDatabase="DataBase/TrainData.txt";
		int train_db=open(TrainDatabase,O_RDWR);
		
		int offset=lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
		if(offset<0)
		{
			write(client_sock,&valid,sizeof(valid));
		}
		else
		{
			valid=1;
			
			
			struct flock lock;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(tnum-1)*sizeof(td);
			lock.l_len=sizeof(td);
			lock.l_pid=getpid();
			fcntl(train_db,F_SETLKW,&lock);
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			read(train_db,&td,sizeof(td));
			td.delete=1;
			lseek(train_db,(tnum-1)*sizeof(td),SEEK_SET);
			write(train_db,&td,sizeof(td));
			lock.l_type=F_UNLCK;
			fcntl(train_db,F_SETLKW,&lock);
			
			write(client_sock,&valid,sizeof(valid));
		}
		close(train_db);
	}
return 0;
}


							
			

				
				
			
			
