#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 

int main(int argc,char* argv[]){
    char* arg;
	int fd = 0;
	if(argc >0){
		arg = argv[0];
        fd =  (int)arg[0] - 48;
    }

	printf("File descriptor is %d\n",fd);
	char* s;
	int bytes_read;
	size_t size=100;
	printf("Please Enter a String (Press Ctrl+C to exit the program): ");

	s = (char *) malloc (size);
	bytes_read = getline(&s,&size,stdin);
	int bytes_wrote = write(fd,s,bytes_read);	
	// if(bytes_wrote>0){
	// 	printf("\nsuccess\n");	
	// }
	close(fd);
	exit(1);
	return 0;
}
