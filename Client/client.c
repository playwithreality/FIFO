/* Program written by Sami Nieminen.
A server-client program based on FIFO pipes.
This is the client program that will give the server data to store.
The program will have two fifo pipes, one for receiving from server and
one for transferring to server.
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>

void InterruptHandler(int);

int main(int argc, char* argv[]) {
	FILE *log_file;
	int fd;
	char *myfifo = "/tmp/server";
	char *myfifo2 = "/tmp/client";
	char filename[250];
	FILE *inputf;

	//making first entry to log file
	log_file = fopen("log events A.txt", "w");
	flock(fileno(log_file) ,LOCK_EX);
	fprintf(log_file, "----------New Run----------\n");
	fprintf(log_file, "Client program started successfully.\n");
	flock(fileno(log_file),LOCK_UN);
	fclose(log_file);

	//prompt user for file to be transferred.
	printf("Enter the file to be transferred as filename.txt. \n");
	printf("The text file should be located in 480329/Client folder.\n");
	printf("Folder 480329/Client contains sample1.txt and sample2.txt files.\n");
	scanf("%s", filename);
	inputf = fopen(filename, "r");

	//confirming that the .txt file is opened
	if (inputf == NULL) {
		printf("Bad filename: exiting program.\n");

		//making log entry before exit
		log_file = fopen("log events A.txt", "a");
		flock(fileno(log_file),LOCK_EX);
		fprintf(log_file, "User entered bad filename making an exit.\n");
		flock(fileno(log_file),LOCK_UN);
		fclose(log_file);
		exit(0);

	}

fclose(inputf);

	// creating the FIFO pipe
	mkfifo(myfifo2, 0666);

	//write "Hi" to the FIFO
	fd = open(myfifo, O_WRONLY);
	write(fd, "Hi", sizeof("Hi"));
	close(fd);

	//FIFO pipe log entries
	log_file = fopen("log events A.txt", "a");
	flock(fileno(log_file) ,LOCK_EX);
	fprintf(log_file, "Created a FIFO pipe.\n");
	fprintf(log_file, "Transferred text to /tmp/.\n");
	flock(fileno(log_file),LOCK_UN);
	fclose(log_file);

	//close FIFO
	unlink(myfifo);

	return 0;
}


void InterruptHandler(int sig) {
  signal(sig, SIG_IGN);
  exit(0);
  signal(SIGINT, InterruptHandler);
  getchar();
}
