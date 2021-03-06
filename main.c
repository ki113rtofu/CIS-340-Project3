//Will only do external commands if path is set to "/bin/:". it does search through
//each path but will only execute if the first one searched is correct path. Working on fix.




#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>

#include "main.h"

int external_command(char *command, char *args[]){
	int status;
	int temp;
	pid_t child;
	child = fork();
			
	if(child == 0){							
		execv(command, args); 
		return -1;							
	}
	else{
		waitpid(-1,&status,NULL);	 
	}
	return 0;	
}
void path(char *arg){
	pathDir = malloc(sizeof(CHAR_MAX));
	strcpy(pathDir, arg);
	
	
	//TODO: throw error for invalid path, also path++, --, display current path
	
	printf("\nCurrent path set: %s\n", arg);
	
}
void quit(){
	QUIT = true;
}

//Run command matching number

void runCommand(char *command, char *arg){
	if(strcmp(command, internal[0]) == 0){
		quit();
	}
	if(strcmp(command, internal[2]) == 0){
		path(arg);
	}
	//add if statement corresponding to command here.
	//numbers will correspond to command order
	// in 'internal' array.
}
//Standard trim function: http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

char *trimwhitespace(char *str){
  
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

void prompt(){
	
	int status = 0;
	char *buf = malloc(sizeof(CHAR_MAX));
	char *internalComp = malloc(sizeof(CHAR_MAX));	
	char *arg = malloc(sizeof(CHAR_MAX));	

	read(0,buf,CHAR_MAX);
	char *trimmed = trimwhitespace(buf);
	int length = strlen(trimmed);
	int numArgs = 1;

	//check for arguments
	for(int i = 0; i < length; i++){
		trimmed[i];
			if(trimmed[i] == ' '){
				trimmed[i] = 0;				
				arg = &trimmed[i+1];
							
			}
	}
	//Run internal command
				
	for(int i = 0; i < 4; i++){  									
		strcpy(internalComp,internal[i]);						
		if(strcmp(trimmed, internalComp) == 0){		
			runCommand(trimmed, arg);
			FOUND = true;				
			break;
		}		
	}
	if(!FOUND){		 
		//Find number of path directories for later use in command search	
		int pathNum = 1;
		int pathLength = strlen(pathDir);		
		for(int i = 0; i < pathLength; i++){
			pathDir[i];
			
			if(pathDir[i] == ':'){
				pathNum++;
			}
		}			
		//Search each individual path for function
		
		char *temp = malloc(sizeof(CHAR_MAX)); 			
		char *curPath = malloc(sizeof(CHAR_MAX));					
		curPath = pathDir;			
		while(pathNum != 0){				
			for(int i = 0; i < pathLength; i++){
				temp[i] = curPath[i];
					
				if(curPath[i] == ':'){
					int count = i;						
					for(int j = 0 ; j < length; j++){
						temp[count] = trimmed[j];
						count++;
					}												
					curPath = &curPath[i+1];						
					break;
				}										
			}				
			int tempLength = strlen(temp);								
			char *parsedArgs[2] = {temp,NULL};				
			int descriptor = external_command(temp, parsedArgs);			
			if(descriptor == 0){
				FOUND = true;
				break;			
			}
			
			//Continue to next path if not found				
			for(int i = 0; i < tempLength; i++){
					temp[i] = ' ';
			}				
			pathNum--;				
		}	
		if(!FOUND){
			printf("\nBad command or invalid path\n");
		}
			
	
	}	
	FOUND = false;
}				
	
int main(){

	do{
		prompt();
	}while(!QUIT);

	return 0;
}
