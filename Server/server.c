/* Program written by Sami Nieminen.
A server-client program based on FIFO pipes.
This is the server program that will read and store data given by the clients.
The program will have two fifo pipes, one for receiving from server and
one for transferrint to server.
*/

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <syslog.h>
#include <pthread.h>
#include "serverfiles.h"

#define MAX_BUFFER 1024

void *transfer_function(void *i);

pthread_mutex_t lock;

int main(int argc, char* argv[]) {

	//file handling variables
	//int fd;
	//char *myfifo = "/tmp/myfifo";
	//char buf[MAX_BUFFER];
	//FILE *output;

	//basic variables for the program
	int fd[512];
	char *myfifo = "/tmp/server";
	char *myfifo2 = "/tmp/client";

	FILE *log_file;

	//making a log file
	log_file = fopen("log events server.txt", "w");
	flock(fileno(log_file) ,LOCK_EX);
	fprintf(log_file, "----------New Run----------\n");
	fprintf(log_file, "Program started successfully.\n");
	flock(fileno(log_file),LOCK_UN);
	fclose(log_file);

	//making server side FIFO pipe and setting FIFO pipe communication rules
	pipe(fd);
	mkfifo(myfifo,0666);
	fd[1] = open(myfifo, O_WRONLY);
	fd[0] = open(myfifo2, O_RDONLY);

	for(;;) {


		//process id related variables
		pid_t pid;
		pid_t sid = 0;

		//setting up thread variables
		int threads = 256, i = 0, c;
		pthread_t *thread = malloc(sizeof(pthread_t)*threads);

		//making a child process
		pid = fork();

		//Informs user of the daemon process id so they can kill easily
		printf("Process id %d, for daemon.\n", pid);
		printf("To kill the daemon use kill %d.\n", pid);

		//checking to make sure nothing went wrong with child creation
		if (pid < 0) {
			printf("Exit failure 1.\n");
			exit(EXIT_FAILURE);
		}

		if (pid > 0) {
			exit(EXIT_SUCCESS);
		}
		//We get here and we are the child process

		//unmasking the file mode
		umask(0);
		printf("We are at unmask.\n");

		//opens a connection to the syslog server
		openlog(argv[0], LOG_NOWAIT|LOG_PID,LOG_USER);
		printf("We are at syslog connection.\n");

		//sends a message to syslog daemon
		syslog(LOG_NOTICE, "Successfully started FIFO daemon\n");

		//a new process group
		sid = setsid();
		printf("We are at new process group.\n");

		//check for new session failure
		if(sid < 0) {
			printf("Failed to create process group.\n");
			syslog(LOG_ERR, "Could not create a new process group\n");
			exit(EXIT_FAILURE);
		}

		//closing standard file descriptors for security and to save system resources
		//close(STDIN_FILENO);
		//close(STDOUT_FILENO);
		//close(STDERR_FILENO);

		//entering the payload part
		for(;;) {
			//making 256 threads
			int *arg = malloc(sizeof(*arg));
			for (i =0; i < threads; i++) {
				*arg = i;
				pthread_create(&thread[i], NULL, transfer_function, arg);
			}

			//joining all 256 threads...
			for (c = 0; c < threads; c++) {
				pthread_join(thread[c], NULL);
			}
			break;

		}

	}
	return 0;
}

void *transfer_function(void *i) {
	int d;
	d = *((int *) i);
	char *msg = "Thread number:";
	int a ;
	pthread_mutex_lock(&lock);
	for (a = 0; a < 10; a++) {
		printf("%s %d count: %d.\n", msg, d, a);
		//sleep(1);
	}
	pthread_mutex_unlock(&lock);
	return 0;
}
