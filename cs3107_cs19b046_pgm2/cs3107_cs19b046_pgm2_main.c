#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <time.h>
#include <signal.h>

// Signal value to break loop of 1st child process
int signalValue = 1;

// args for dictLogic process 
char argOfFile[1]; 
// char argForOutput[1];

// custom pipe -> simply opening same file with different flags
int customPipe(int fd[2]){
	remove("foo.txt");

	fd[1] = open("foo.txt",O_WRONLY | O_CREAT | O_TRUNC,0700);
	fd[0] = open("foo.txt",O_RDONLY | O_CREAT | O_TRUNC,0700);
	
	// printf("\nfd0 is %d and fd1 is %d\n",fd[0],fd[1]);

	if(fd[0]>-1 && fd[1]>-1){
		return 1;
	}
	return -1;
	
}

// overriding signal interrupt
void signal_handler(int signal)
{
	signalValue = 0;
	char choice;
	while(choice!='y' && choice !='n'){
		printf("\nDo u want to save your input in a file?: \n\t1. press 'y' for yes\n\t2. press 'n' for no\n");
		scanf("%c",&choice);
		if(choice=='y'){
			argOfFile[0] = 'y';
		}
		else if (choice=='n'){
			argOfFile[0]='n';
		}
		else{
			printf("\nPlease Enter 'y' or 'n'\n");
		}
	}
}

int main(int argc,char* argv[]){
	// -----------------------------------TIMESTAMPS--------------------------------//
	struct tm* local;
    time_t t = time(NULL);
  
    // Get the localtime
    local = localtime(&t);
  
    printf("\nLocal time and date: %s\n",
	asctime(local));

	// -----------------------------------TIMESTAMPS--------------------------------//

	// Overriding signal here
	signal(SIGINT,  signal_handler);


	// Reinitialize outputFile
	remove("outputFile.txt");
	open("outputFile.txt",O_RDWR | O_CREAT | O_TRUNC,0700);
	while(signalValue){
		
		int fd[2];
		if(customPipe(fd)==-1){
			perror("Something Happend");
			return 1;
		}


		int pid1 = fork();
		if(pid1<0){
			return 2;
		}
		
		if(pid1==0){
			close(fd[0]);
			char execStr[2];
			snprintf(execStr,2+1, "%d",fd[1]);
			// printf("\nfd is here: %c\n",execStr[0]);
			execlp("./input.out",execStr,NULL);
			return 3;
		}

		waitpid(pid1,NULL,0);
		
		// if(signalValue==1){

		// }
		

		int pid2 = fork();
			
		if(pid2<0){
			return 4;
		}
		
		if(pid2==0){
			close(fd[1]);
			char execStr[2];
			snprintf(execStr,2+1, "%d",fd[0]);
			// printf("\nfd for process2 %c\n",execStr[0]);
			if(signalValue==0){
				execlp("./logic.out",execStr,argOfFile,NULL);
			}
			else{
				execlp("./logic.out",execStr,NULL);

			}
			return 5;
		}
		
		//Closing fd of parent
		close(fd[0]);
		close(fd[1]);
		
		waitpid(pid2,NULL,0);
		
		remove("foo.txt");
	}
	
	return 0;
}
