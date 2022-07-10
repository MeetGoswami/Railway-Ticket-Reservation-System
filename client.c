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

int main_menu(int serv_sock);
int login(int sd);
int signup(int sd);
int view_profile(int sd);
int update_profile(int sd);
int book_ticket(int sd);
int view_booking(int sd);
int update_booking(int sd);
int cancel_booking(int sd);
int customer_services(int serv_sock);
int add_train(int sd);
int modify_train(int sd);
int remove_train(int sd);
int manage_trains(int sd);
int remove_user(int sd);
int modify_user(int sd);;
int manage_users(int sd);
int admin_services(int serv_sock);


int main()
{
	struct sockaddr_in serv;
	int sd;
	sd = socket(AF_UNIX,SOCK_STREAM,0);
	
	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port=htons(8000);
	
	while(connect(sd, (void *)(&serv), sizeof(serv))!=0)
	{
		printf("\t\tWaiting for server    ");
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
	
	printf("\n\t\tConnected to server\n");
	main_menu(sd);
	return 0;
}
	
	
	int main_menu(int serv_sock)
	{
		int choice=0;
		printf("\n\t\tWELCOME TO ONLINE RAILWAY TICKET RESERVATION SYSTEM\n");
		while(choice!=3)
		{
			
			
			printf("\n\n\t\tChoose from the below option:\n");
			printf("\t\t1. Login\n");
			printf("\t\t2. Sign Up\n");
			printf("\t\t3. Exit\n");
			printf("\t\tEnter your choice : ");
			scanf("%d",&choice);
			write(serv_sock, &choice , sizeof(int));
			if(choice==1)
			{
				int status=login(serv_sock);
				if(status==1)
				{
					customer_services(serv_sock);
				}
				else if(status==2)
				{
					customer_services(serv_sock);
				}
				else if(status==3)
				{
					admin_services(serv_sock);
				}
				else
				{}
			}
			else if(choice==2)
			{
				signup(serv_sock);
			}
			else if(choice>3 || choice <=0)
			{
				printf("\t\t!!! INVALID INPUT !!!\n");
			}
			else
			{}
		}
		return 0;
	}
// -----------------------------------------------LOGIN PAGE FUNCTION-------------------------------------------------------------------------------------------------------------------
		int login(int sd)
		{
			int utype=-1;
			while(utype!=0)
			{
				printf("\n\n\t\tLOGIN PAGE\n");
				printf("\t\tChoose from the below option :\n");
				printf("\t\t1. Customer Login\n");
				printf("\t\t2. Agent Login\n");
				printf("\t\t3. Admin Login\n");
				printf("\t\t0. Go back to Main Menu\n");
				printf("\t\tEnter your choice : ");
				scanf("%d",&utype);
				write(sd,&utype,sizeof(int));
				if(utype==0)
				{
					return 0;
				}
				else if(utype>0 && utype<=3)
				{
					char uname[30];
					char passwd[30];
					int valid=0;
					printf("\n\n\t\tENTER THE CREDENTIALS\n");
					printf("\t\tUsername : ");
					scanf(" %[^\n]",uname);
					printf("\t\tPassword : ");
					//char *passin=getpass("Password : ");
					//strcpy(passwd,passin);
					scanf(" %[^\n]",passwd);
					write(sd, uname, sizeof(uname));
					write(sd, passwd, sizeof(passwd));
					//write(sd, &utype, sizeof(utype));
					read(sd, &valid, sizeof(valid));
					if(valid==1)
					{
						printf("\n\t\tLOGIN SUCCESSFUL");
						return utype;
					}
					else if(valid==0)
					{
						printf("\n\t\tINVALID CREDENTIALS. LOGIN FAILED.\n");
					}
					else if(valid==2)
					{
						printf("\n\t\tACCOUNT ALREADY LOGGED IN. PLEASE LOGOUT FROM OTHER DEVICE AND TRY AGAIN.\n");
					}
					else
					{
					}
				}
				else
				{
					printf("\n\t\t!!! INVALID INPUT !!!\n");
				}
			}
			return 0;
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------SIGNUP FUNCTION------------------------------------------------------------------------------------------------------------
	int signup(int sd)
	{
		int utype=-1;
			while(utype!=0)
			{
				printf("\n\n\t\tChoose from the below option :\n");
				printf("\t\t1. Customer Sign Up\n");
				printf("\t\t2. Agent Sign Up\n");
				printf("\t\t3. Admin Sign Up\n");
				printf("\t\t0. Go back to Main Menu\n");
				printf("\t\tEnter your choice : ");
				scanf("%d",&utype);
				write(sd, &utype, sizeof(int));
				if(utype==0)
				{
					return 0;
				}
				else if(utype>0 && utype<4)
				{
					if(utype==3)
					{
						char adminkey[20];
						int keyValid=0;
						printf("\n\t\tEnter the secret Admin key : ");
						scanf(" %[^\n]",adminkey);
						write(sd, adminkey, sizeof(adminkey));
						read(sd, &keyValid, sizeof(int));
						if(keyValid==0)
						{
							printf("\n\t\tINVALID ADMIN KEY\n");
							utype=10;
						}
					}
					if(utype>0 && utype<4)	
					{
						struct userDetails ud;
						ud.login=0;
						ud.delete=0;
						ud.utype=utype;
						printf("\n\t\tENTER THE FOLLOWING DETAILS : \n");
						printf("\t\tName : ");
						scanf(" %[^\n]",ud.name);
						printf("\t\tGender (M/F/O) : ");
						scanf(" %[^\n]",&ud.gender);
						printf("\t\tAge : ");
						scanf("%d",&ud.age);
						printf("\t\tMobile Number : ");
						scanf(" %[^\n]",ud.mob);
						printf("\t\tEmail : ");
						scanf(" %[^\n]",ud.email);
						
						printf("\n\t\tCREATE LOGIN CREDENTIALS\n");
						printf("\t\tUsername : ");
						scanf(" %[^\n]", ud.uname);
						printf("\t\tPassword : ");
						scanf(" %[^\n]",ud.passwd);
						int accStatus=0;
						write(sd, &ud, sizeof(ud));
						read(sd, &accStatus, sizeof(accStatus));
						if(accStatus==0)
						{
							printf("\n\t\tThis username is not available. Please select a different username");
							printf("\t\tACCOUNT CREATION FAILED\n");
						}
						else
						{
							printf("\n\t\tACCOUNT CREATED SUCCESSFULLY\n");
							return 0;
						}
						
					}
						
				}
				
				else
				{
					printf("\n\t\t!!! INVALID INPUT !!!\n");
				}
			}
			return 0;
		}
//------------------------------------------------------------CUSTOMER SERVICES FUNCTION-------------------------------------------------------------------------------
		int view_profile(int sd)
		{
			struct userDetails udet;
			read(sd, &udet, sizeof(udet));
			printf("\n\t\tUSER DETAILS\n");
			printf("\t\tUsername : %s\n",udet.uname);
			printf("\t\tName : %s\n",udet.name);
			printf("\t\tAge : %d\n",udet.age);
			printf("\t\tGender : %c\n",udet.gender);
			printf("\t\tMobile : %s\n",udet.mob);
			printf("\t\tEmail : %s\n", udet.email);
			
			return 0;
		}
		
		
		int update_profile(int sd)
		{
			int choice=-1;
			struct userDetails udet;
			read(sd, &udet, sizeof(udet));
			printf("\n\t\tCURRENT USER DETAILS\n");
			printf("\t\t1.Username : %s\n",udet.uname);
			printf("\t\t2.Name : %s\n",udet.name);
			printf("\t\t3.Age : %d\n",udet.age);
			printf("\t\t4.Gender : %c\n",udet.gender);
			printf("\t\t5.Mobile : %s\n",udet.mob);
			printf("\t\t6.Email : %s\n", udet.email);
			printf("\t\t7. Password : \n");
			
			printf("\n\t\tEnter the field number that you would like to update (Enter 0 to Go Back to previous menu) : ");
			scanf("%d",&choice);
			write(sd, &choice, sizeof(int));
			if(choice==1)
			{
				printf("\n\t\tUsername once set cannot be changed.\n");
			}
			else if(choice==2)
			{
				printf("\n\t\tEnter the new details\n");
				printf("\t\tName : ");
				scanf(" %[^\n]",udet.name);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==3)
			{
				printf("\n\t\tEnter the new details\n");
				printf("\t\tAge : ");
				scanf("%d",&udet.age);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==4)
			{
				printf("\n\t\tEnter the new details\n");
				printf("\t\tGender (M/F/O) : ");
				scanf("%c",&udet.gender);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==5)
			{
				printf("\n\t\tEnter the new details\n");
				printf("\t\tMobile : ");
				scanf(" %[^\n]",udet.mob);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==6)
			{
				printf("\n\t\tEnter the new details\n");
				printf("\t\tEmail : ");
				scanf(" %[^\n]",udet.email);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==7)
			{
				printf("\n\t\tEnter the new details\n");
				char passwd1[30], passwd2[30];
				printf("\t\tNew Password : ");
				scanf(" %[^\n]",udet.passwd);
				write(sd, &udet,sizeof(udet));
				printf("\n\t\tDetails updated successfully\n");
			}
			else if(choice==0)
			{}
			else
			{
				printf("\n\t\t !!! INVALID CHOICE !!!\n");
			}
			return 0;
		}
		
		int book_ticket(int sd)
		{
			int choice=-1;
			while(choice!=0)
			{
				printf("\n\t\tSEARCH TRAINS\n");
				printf("\t\t1. Search by Train No\n");
				printf("\t\t2. Search by Location\n");
				printf("\t\t0. Go Back\n");
				printf("\t\tEnter your choice : ");
				scanf("%d",&choice);
						
				if(choice==1)
				{
					write(sd, &choice, sizeof(choice));
					int trainNo=0;
					printf("\n\t\tEnter train no : ");
					scanf("%d",&trainNo);
					write(sd,&trainNo,sizeof(trainNo));
					int searchResult=0;
					read(sd,&searchResult,sizeof(searchResult));
					if(searchResult==0)
					{
						printf("\n\t\tNO TRAIN AVAILABLE WITH THE GIVEN TRAIN NUMBER\n");
					}
					else
					{
						printf("%-10s  %-20s  %-15s  %-15s  %-15s  %-10s\n","Train No.","Train Name", "From","To","Available Seats","Total Seats");
						struct trainDetails td;
						read(sd,&td,sizeof(td));
						printf("%-10d  %-20s  %-15s  %-15s  %-15d  %-10d\n", td.train_num, td.train_name, td.source, td.dest, td.available_seats, td.total_seats);
						printf("\n\t\tDo you want to book seats in this train?");
						printf("\t\t1. Yes\n");
						printf("\t\t2. No\n");
						printf("\t\tEnter your choice : ");
						int confTrain=0;
						scanf("%d",&confTrain);
						write(sd,&confTrain,sizeof(confTrain));
						if(confTrain==1)
						{
							int bookSeats,bookStatus;
							char pname[50];
							printf("\n\t\tEnter passenger name : ");
							scanf(" %[^\n]",pname);
							printf("\t\tEnter the number of seats to book : ");
							scanf("%d",&bookSeats);
							write(sd,pname,sizeof(pname));
							write(sd,&bookSeats,sizeof(bookSeats));
							read(sd, &bookStatus,sizeof(bookStatus));
							if(bookStatus==0)
							{
								struct bookingDetails bd;
								read(sd, &bd,sizeof(bd));
								printf("\n\t\tBOOKING CONFIRMED\n");
								printf("\n\t\tBOOKING DETAILS ARE:\n");
								printf("\t\tPNR : %ld\n",bd.pnr);
								printf("\t\tPassenger name : %s\n",bd.pname);
								printf("\t\tTrain No. : %d\n", bd.train_num);
								printf("\t\tTrain Name : %s\n",bd.train_name);
								printf("\t\tSource : %s\n", bd.source);
								printf("\t\tDestination : %s\n", bd.dest);
								printf("\t\tNumber of seats booked : %d\n", bd.booked_seats);
							}
							else
							{
								printf("\n\t\tBOOKING FAILED\n");
							}
						}
						else if(confTrain==2)
						{
							
						}
						else
						{
							printf("\n\t\t!!! INVALID INPUT !!!\n");
						}
					}
				}
				else if(choice==2)
				{
					write(sd, &choice, sizeof(choice));
					char source[30], dest[30];
					printf("\n\t\tEnter source : ");
					scanf(" %[^\n]", source);
					
					printf("\t\tEnter destination : ");
					scanf(" %[^\n]", dest);
					
					write(sd, source, sizeof(source));
					write(sd, dest, sizeof(dest));
					int searchResult=0;
					read(sd,&searchResult,sizeof(int));
					if(searchResult==0)
					{
						printf("\n\t\tNO TRAIN AVAILABLE WITH THE GIVEN SOURCE AND DESTINATION\n");
					}
					else
					{
						printf("%-10s  %-20s  %-15s  %-15s  %-15s  %-10s\n","Train No.","Train Name", "From","To","Available Seats","Total Seats");
						struct trainDetails td;							
						for(int i=0;i<searchResult;i++)
						{
							read(sd,&td,sizeof(td));
							printf("%-10d  %-20s  %-15s  %-15s  %-15d  %-10d\n",td.train_num, td.train_name, td.source, td.dest, td.available_seats, td.total_seats);
						}
						
						printf("\n\t\tEnter the Train No. to select a train (Enter 0 to Go Back): ");
						int book_num;
						scanf("%d",&book_num);
						write(sd, &book_num, sizeof(int));
						if(book_num!=0)
						{
							char pname[50];
							printf("\n\t\tEnter passenger name : ");
							scanf(" %[^\n]",pname);
							printf("\t\tEnter the number of seats to book : ");
							int bookSeats,bookStatus;
							scanf("%d",&bookSeats);
							write(sd,&bookSeats,sizeof(int));
							read(sd, &bookStatus,sizeof(int));
							if(bookStatus==0)
							{
								struct bookingDetails bd;
								read(sd, &bd,sizeof(bd));
								printf("\n\t\tBOOKING CONFIRMED\n");
								printf("\n\t\tBOOKING DETAILS ARE:\n");
								printf("\t\tPNR : %ld\n",bd.pnr);
								printf("\t\tPassenger name : %s\n",bd.pname);
								printf("\t\tTrain No. : %d\n", bd.train_num);
								printf("\t\tTrain Name : %s\n",bd.train_name);
								printf("\t\tSource : %s\n", bd.source);
								printf("\t\tDestination : %s\n", bd.dest);
								printf("\t\tNumber of seats booked : %d\n", bd.booked_seats);
							}
							else
							{
								printf("\n\t\tBOOKING FAILED\n");
							}
						}
						else
						{
						}
					}
				}
				else if(choice==0)
				{
					write(sd, &choice, sizeof(choice));
				}
				else
				{
					printf("\n\t\t!!! INVALID CHOICE !!!\n");
				}
			}
			return 0;
		}
		
		
		int view_booking(int sd)
		{
			int totalPrevBooking=0;
			read(sd, &totalPrevBooking, sizeof(totalPrevBooking));
			printf("Total previous bookings = %d\n",totalPrevBooking);	
			struct bookingDetails bd;
			printf("%-8s  %-10s  %-20s  %-10s  %-20s  %-15s  %-15s  %-15s\n","Sr. No.","PNR","Passenger Name","Train No.","Train Name", "From","To","Seats Booked");							
			for(int i=0;i<totalPrevBooking;i++)
			{
				read(sd, &bd,sizeof(bd));
				printf("%-8d  %-10ld  %-20s  %-10d  %-20s  %-15s  %-15s  %-15d\n",(i+1),bd.pnr,bd.pname,bd.train_num,bd.train_name,bd.source,bd.dest,bd.booked_seats);
			}
			return 0;
		}
		
		
		int update_booking(int sd)
		{
			long pnr_num;
			printf("\n\t\tEnter the PNR number : ");
			scanf("%ld",&pnr_num);
			write(sd, &pnr_num, sizeof(pnr_num));
			int valid=-1;
			read(sd, &valid,sizeof(valid));
			if(valid==0)
			{
				struct bookingDetails bd;
				read(sd, &bd,sizeof(bd));
				printf("\n\t\tBOOKING DETAILS\n");
				printf("\t\tPNR : %ld\n",bd.pnr);
				printf("\t\tPassenger name : %s\n",bd.pname);
				printf("\t\tTrain No. : %d\n", bd.train_num);
				printf("\t\tTrain Name : %s\n",bd.train_name);
				printf("\t\tSource : %s\n", bd.source);
				printf("\t\tDestination : %s\n", bd.dest);
				printf("\t\tNumber of seats booked : %d\n", bd.booked_seats);			
				printf("\n");
				printf("\t\tWhat would you like to update\n");
				printf("\t\t1.Passenger Name\n");
				printf("\t\t2.Number of seats booked\n");
				printf("\n\t\tEnter your choice (Enter 0 to Go Back) : ");
				int updateChoice=-1;
				scanf("%d",&updateChoice);
				write(sd, &updateChoice,sizeof(updateChoice));
				if(updateChoice==1)
				{
					printf("\n\t\tEnter Passenger Name : ");
					scanf(" %[^\n]",bd.pname);
					write(sd, &bd, sizeof(bd));
					printf("\n\t\tINFORMATION UPDATED SUCCESSFULLY\n");
				}
				else if(updateChoice==2)
				{
					printf("\n\t\tEnter the number of seats : ");
					int newSeats=-1;
					scanf("%d",&newSeats);
					write(sd,&newSeats,sizeof(int));
					int status=-1;
					read(sd,&status,sizeof(status));
					if(status==0)
					{
						printf("\n\t\tINFORMATION UPDATED SUCCESSFULLY\n");
					}
					else
					{
						printf("\n\t\tINFORMATION UPDATION FAILED\n");
					}
				}
				else if(updateChoice==0)
				{
				}
				else
				{
					printf("\n\t\t!!! INVALID INPUT !!!\n");
				}
			}
			else
			{
				printf("\n\t\tINAVLID PNR NUMBER\n");
			}
			return 0;
		}
		
		int cancel_booking(int sd)
		{
			long pnr_num;
			printf("\n\t\tEnter the PNR number : ");
			scanf("%ld",&pnr_num);
			write(sd, &pnr_num, sizeof(pnr_num));
			int valid=-1;
			read(sd, &valid,sizeof(valid));
			if(valid==0)
			{
				printf("\n\t\tBOOKING CANCELLATION SUCCESSFULL\n");
			}
			else
			{
				printf("\n\t\tBOOKING CANCELLATION FAILED\n");
			}
			return 0;
		}
		
		int customer_services(int serv_sock)
		{
			int serviceChoice=-1;
			while(serviceChoice!=0)
			{
				serviceChoice=-1;
				printf("\n\t\tChoose from below options :\n");
				printf("\t\t1. View Profile\n");
				printf("\t\t2. Update Profile\n");
				printf("\t\t3. Book Ticket\n");
				printf("\t\t4. View Previous Bookings\n");
				printf("\t\t5. Update Booking\n");
				printf("\t\t6. Cancel Booking\n");
				printf("\t\t0. Logout\n");
				printf("\t\tEnter your choice : ");
				scanf("%d",&serviceChoice);
				write(serv_sock, &serviceChoice, sizeof(serviceChoice));
				if(serviceChoice==0)
				{
					return 0;
				}
				else if(serviceChoice==1)
				{
					view_profile(serv_sock);
				}
				else if(serviceChoice==2)
				{
					update_profile(serv_sock);
				}
				else if(serviceChoice==3)
				{
					book_ticket(serv_sock);
				}	
				
				else if(serviceChoice==4)
				{
					view_booking(serv_sock);
				}
				
				else if(serviceChoice==5)
				{
					update_booking(serv_sock);
				}
				
				else if(serviceChoice==6)
				{
					cancel_booking(serv_sock);
				}
				
				else
				{
					printf("\n\t\t!!! INVALID CHOICE !!!\n");
				}
			}
			return 0;
		}
//--------------------------------------------------------ADMIN SERVICES FUNCTION--------------------------------------------------------------------------------------- 
		
		int add_train(int sd)
		{
			struct trainDetails new_train;
			printf("\n\t\tEnter the following train details : \n");
			printf("\t\tTrain Name : ");
			scanf(" %[^\n]",new_train.train_name);
			printf("\t\tSource : ");
			scanf(" %[^\n]",new_train.source);
			printf("\t\tDestination : ");
			scanf(" %[^\n]",new_train.dest);
			printf("\t\tTotal Seats : ");
			scanf("%d",&new_train.total_seats);
			new_train.available_seats=new_train.total_seats;
			new_train.train_num=-1;
			write(sd, &new_train, sizeof(new_train));
			int tnum=-1;
			read(sd, &tnum,sizeof(int));
			if(tnum>0)
			{
				printf("\n\t\tTRAIN ADDED SUCCESSFULLY\n");
				printf("\t\tTRAIN NUMBER FOR THE ADDED TRAIN IS %d\n",tnum);
			}
			else
			{
				printf("\n\t\tTRAIN NOT ADDED. OPERATION FAILED\n");
			}
			return 0;
		}
		
		int modify_train(int sd)
		{
			printf("\n\t\tEnter Train Number : ");
			int tnum;
			scanf("%d",&tnum);
			write(sd,&tnum,sizeof(tnum));
			int validTnum=-1;
			read(sd,&validTnum,sizeof(validTnum));
			if(validTnum==0)
			{
				struct trainDetails td;
				read(sd,&td,sizeof(td));
				printf("\n\t\tTRAIN DETAILS\n");
				
				printf("\t\t1.Train Number : %d\n",td.train_num);
				printf("\t\t2.Train Name : %s\n",td.train_name);
				printf("\t\t3.Source : %s\n",td.source);
				printf("\t\t4.Destination : %s\n",td.dest);
				printf("\t\t5.Available Seats : %d\n",td.available_seats);
				printf("\t\t6.Total Seats : %d\n",td.total_seats);
						
				printf("\n\t\tEnter the field number that you want to modify : ");
				int fnum=-1;
				scanf("%d",&fnum);
				write(sd,&fnum,sizeof(fnum));
				if(fnum==1)
				{
					printf("\n\t\tTrain number once set cannot be modified\n");
				}
				else if(fnum==2)
				{
					int ack=0;
					printf("\n\t\tEnter new Name : ");
					scanf(" %[^\n]",td.train_name);
					write(sd,&td,sizeof(td));
					read(sd,&ack,sizeof(ack));
					printf("\n\t\tTRAIN DETAILS UPDATED\n");
				}
				else if(fnum==3)
				{
					int ack=0;
					printf("\n\t\tEnter new source : ");
					scanf(" %[^\n]",td.source);
					write(sd,&td,sizeof(td));
					read(sd,&ack,sizeof(ack));
					printf("\n\t\tTRAIN DETAILS UPDATED\n");
				}
				else if(fnum==4)
				{
					int ack=0;
					printf("\n\t\tEnter new destination : ");
					scanf(" %[^\n]",td.dest);
					write(sd,&td,sizeof(td));
					read(sd,&ack,sizeof(ack));
					printf("\n\t\tTRAIN DETAILS UPDATED\n");
				}
				else if(fnum==5 || fnum==6)
				{
					int seat_opt=-1;
					printf("\n\t\tEnter from below option : \n");
					printf("\t\t1. Add seats\n");
					printf("\t\t2. Remove seats\n");
					printf("\n\t\tEnter your choice : ");
					scanf("%d",&seat_opt);
					write(sd,&seat_opt,sizeof(seat_opt));
					if(seat_opt==1)
					{
						int ack=0;
						int num;
						printf("\n\t\tEnter the number of seats to add: ");
						scanf("%d",&num);
						write(sd,&num,sizeof(num));
						if(num>0)
						{
							td.available_seats+=num;
							td.total_seats+=num;
							write(sd,&td,sizeof(td));
							read(sd,&ack,sizeof(ack));
							printf("\n\t\tTRAIN DETAILS UPDATED\n");
						}
						else
						{
							printf("\n\t\tINVALID INPUT. UPDATION FAILED\n");
						}
					}
					else if(seat_opt==2)
					{
						int ack=0;
						int num;
						printf("\n\t\tEnter the number of seats to remove: ");
						scanf("%d",&num);
						write(sd,&num,sizeof(num));
						if(num>0 && num<=td.available_seats)
						{
							td.available_seats-=num;
							td.total_seats-=num;
							write(sd,&td,sizeof(td));
							read(sd,&ack,sizeof(ack));
							printf("\n\t\tTRAIN DETAILS UPDATED\n");
						}
						else
						{
							printf("\n\t\tINVALID INPUT. UPDATION FAILED\n");
						}
					}
					else
					{
						printf("\n\t\tINVALID INPUT. UPDATION FAILED\n");		
					}
				}
				
				else
				{
					printf("\n\t\t!!! INVALID INPUT !!!\n");
				}
			}
			else
			{
				printf("\n\t\tINVALID TRAIN NUMBER\n");
			}
			return 0;
		}
		
		
		int remove_train(int sd)
		{
			printf("\n\t\tEnter Train Number : ");
			int tnum;
			scanf("%d",&tnum);
			write(sd,&tnum,sizeof(tnum));
			int validTnum=-1;
			read(sd,&validTnum,sizeof(int));
				
			if(validTnum==0)
			{
				printf("\n\t\tTRAIN REMOVED SUCCESSFULLY\n");
			}
			else
			{
				printf("\n\t\tINVALID TRAIN NUMBER\n");
			}
			return 0;
		}
		
		int manage_trains(int sd)
		{
			int choice=-1;
			while(choice!=0)
			{
				printf("\n\t\tChoose from below options :\n");
				printf("\t\t1. Add Train\n");
				printf("\t\t2. Modify Train\n");
				printf("\t\t3. Remove Train\n");
				printf("\t\t0. Go Back\n");
				printf("\n\t\tEnter your choice : ");
				scanf("%d",&choice);
				write(sd,&choice,sizeof(choice));
				if(choice==1)
				{
					add_train(sd);
				}
				else if(choice==2)
				{
					modify_train(sd);
				}
				
				else if(choice==3)
				{
					remove_train(sd);
				}
				else if(choice==0)
				{
						
				}
				else 
				{
					printf("\n\t\t!!! INVALID CHOICE !!!\n");
				}
			}
			return 0;
		}

		
		int remove_user(int sd)
		{
			char username[30];
			int valid=-1;
			printf("\n\t\tEnter username : ");
			scanf(" %[^\n]",username);
			write(sd,username,sizeof(username));
					
			read(sd,&valid,sizeof(valid));
			if(valid==0)
			{
				printf("\n\t\tUSER DELETED SUCCESSFULLY\n");
			}
			else
			{
				printf("\n\t\tTHIS USERNAME DOES NOT EXIST\n");
			}
			return 0;
		}
		
		int modify_user(int sd)
		{
			char username[30];
			int valid=-1;
			printf("\n\t\tEnter username : ");
			scanf(" %[^\n]",username);
			write(sd,username,sizeof(username));
					
			read(sd,&valid,sizeof(valid));
			if(valid==0)
			{
				update_profile(sd);
			}
			else
			{
				printf("\n\t\tTHIS USERNAME DOES NOT EXIST\n");
			}
			return 0;
		}
		
		
		int manage_users(int sd)
		{
			int choice=-1;
			while(choice!=0)
			{
				printf("\n\t\tChoose from below options :\n");
				printf("\t\t1. Add User\n");
				printf("\t\t2. Modify User\n");
				printf("\t\t3. Remove User\n");
				printf("\t\t0. Go Back\n");
				printf("\n\t\tEnter your choice : ");
				scanf("%d",&choice);
				write(sd,&choice,sizeof(choice));
				
				if(choice==1)
				{
					signup(sd);
				}
				
				else if(choice==2)
				{
					modify_user(sd);
				}
				else if(choice==3)
				{
					remove_user(sd);
				}
				else if(choice==0)
				{
					break;
				}
				else
				{
				}
			}
			return 0;
		}
				
		
		int admin_services(int serv_sock)
		{
			
			int serviceChoice=-1;
			while(serviceChoice!=0)
			{
				printf("\n\t\tChoose from below options :\n");
				printf("\t\t1. Manage Trains\n");
				printf("\t\t2. Manage Users\n");
				printf("\t\t0. Logout\n");
				printf("\n\t\tEnter your choice : ");
				scanf("%d",&serviceChoice);
				write(serv_sock, &serviceChoice, sizeof(serviceChoice));
				if(serviceChoice==0)
				{
					return 0;
				}
				else if(serviceChoice==1)
				{
					manage_trains(serv_sock);
				}
										
				else if(serviceChoice==2)
				{
					manage_users(serv_sock);
				
				}
			}
			return 0;
		}
