#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	mkdir("DataBase",0777);
	perror("mkdir");
	open("DataBase/UserData.txt", O_RDWR | O_CREAT, S_IRWXU);
	open("DataBase/TrainData.txt", O_RDWR | O_CREAT, S_IRWXU);
	open("DataBase/BookingData.txt", O_RDWR | O_CREAT, S_IRWXU);
	int tnum_db=open("DataBase/train_number.txt", O_RDWR | O_CREAT, S_IRWXU);
	perror("open");
	int pnr_db=open("DataBase/PNR.txt", O_RDWR | O_CREAT, S_IRWXU);
	perror("open");
	
	int tnum=0;
	long pnr=0;
	
	write(tnum_db,&tnum,sizeof(tnum));
	perror("write");
	write(pnr_db,&pnr,sizeof(pnr));
	perror("write");
	
	perror("open");
}

