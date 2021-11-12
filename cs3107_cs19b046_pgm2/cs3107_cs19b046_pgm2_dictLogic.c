#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <string.h>

// get next word from the given file descriptor(used for dictionary)
int getNextWord(char nextWord[],int fd){
    char *s;
    int wordIndex = 0;
    int bytes_read;
	size_t size=100;
	s = (char *) malloc (size);
    while(bytes_read = read(fd,s,sizeof(char))>0){
        nextWord[wordIndex]=s[0];
        if(s[0]==' ' || s[0]=='\n'){
            nextWord[wordIndex] = 0;
            return 1;            
        }
        wordIndex++;
    }
    return -1;
}

// Comparing for the longest match -> length wise
int compare(char* input,char* dict){
    int length = 0;
    while(length < strlen(input) && length<strlen(dict)){
        if(input[length]==dict[length]){
            length++;
        }
        else{
            return length;
        }
    }
    return length;
}

int openDict(){
    int dictFd;
    if((dictFd = open("dict.txt",O_RDONLY)) <0){
        perror("File Not Present - dict.txt");
        return -1;
    }
    dup2(dictFd,10);
    close(dictFd);
    return 10;
}

int main(int argc,char* argv[]){
    char* arg;
	int fd = 0;
	if(argc >0){
		arg = argv[0];
        fd =  (int)arg[0] - 48;

    }
    // if(fd == 0){
    //     fd = open("test.txt",O_RDONLY);
    // }

	char* s;
    char wordList[1000][1000];
    int noOfWords=0;
	size_t size=100;
	s = (char *) malloc (size);
    int bytes_read;
    char nextWord[1000];
    int wordIndex = 0;

	while(bytes_read = read(fd,s,sizeof(char))>0){
        nextWord[wordIndex]=s[0];
        if(s[0]==' ' || s[0]=='\n'){
            nextWord[wordIndex] = 0;
            strcpy(wordList[noOfWords],nextWord);
            noOfWords++;
            wordIndex=0;
            continue;
        }
        wordIndex++;
    }

    int dictFd = openDict();
    if(dictFd<0){
        perror("dict.txt not found");
        return 2;
    }

    char newWordList[1000][1000];
    for(int i=0;i<noOfWords;i++){
        strcpy(newWordList[i],wordList[i]);
    }

    for(int i=0;i<noOfWords;i++){
        int dictFd = openDict();
        if(dictFd<0){
            return 2;
        } 
        int longestMatch = 0,currentMatch;
        while(getNextWord(nextWord,dictFd)>0){

            currentMatch = compare(wordList[i],nextWord);              
            if(currentMatch>longestMatch){
                strcpy(newWordList[i],nextWord);        
                longestMatch = currentMatch;
            }     
            else if(currentMatch==longestMatch){
                if(!longestMatch==0){
                    if(strcmp(newWordList[i],nextWord)>1){
                        strcpy(newWordList[i],nextWord);
                    }
                }
            }   

        }
        close(dictFd);
    }


    char choice;

    if(argc<=1){

        printf("\n--------------------------START OF FORMATTED INPUT------------------------\n\n");
        for(int i=0;i<noOfWords;i++){
            printf("%s ",newWordList[i]);
        }

        printf("\n\n--------------------------END OF FORMATTED INPUT----------------------------\n");
        
        
        do{
            printf("\nChoose Whether U want to see raw input as output or dictionary formatted Output: \n\t1. press 'y' for formatted input\n\t2. press 'n' for raw input\n");
            scanf("%c",&choice);
            if(choice!='y' && choice !='n'){
                printf("\nPlease Enter 'y' or 'n'\n");
            }
        }while(choice!='y' && choice !='n');
    }
    


    char resultArr[1000][1000];
    // arg2 corresponds to raw or formatted input
    // printf("\n%s %s\n",argv[1],argv[2]);
    if(choice=='y'){
        // outputWrapper(argv[1],noOfWords,newWordList);
        for(int i=0;i<noOfWords;i++){
            strcpy(resultArr[i],newWordList[i]);
        }
    }
    else{
        // outputWrapper(argv[1],noOfWords,wordList);
        for(int i=0;i<noOfWords;i++){
            strcpy(resultArr[i],wordList[i]);
        }

    }
    // printf("\n--------------------------START-------------------\n");
    // for(int i=0;i<noOfWords;i++){
    //     printf("%s ",resultArr[i]);
    // }

    // printf("\n--------------------------END---------------------\n");
    int writeFD = -1;
    int bytes_written;
    int readFD = -1;

    if(argc>1 && argv[1][0]=='y'){

        printf("Items have been written to outputFile.txt");
    }
    else if(argc>1 && argv[1][0]=='n'){
        char *stream;
        size_t streamSize=100;
        int bytes_read;
        stream = (char*)malloc(streamSize);
        readFD = open("outputFile.txt",O_RDWR);
        while((bytes_read = read(readFD,stream,sizeof(char))>0)){
            printf("%c",stream[0]);
        }
        close(readFD);
        remove("outputFile.txt");
    }
    else{        
        // printf("Here2\n");

        // This works only when creat and trunc flags are not given.. not sure why!!
        writeFD = open("outputFile.txt",O_RDWR);

        for(int i=0;i<noOfWords;i++){
            // printf("%s ",resultArr[i]);
            lseek(writeFD,0,SEEK_END);
            bytes_written = write(writeFD,resultArr[i],strlen(resultArr[i]));
            bytes_written = write(writeFD," ",1);
        }
        bytes_written = write(writeFD,"\n",1);
        close(writeFD);
    }

    close(fd);
    return 0;    
}