/*
 * main.cpp
 *
 *  Created on: May 25, 2020
 *      Author: amaldev
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define SHELL_BUF_SIZE 1024
#define SHELL_DELIM "\t\n\r\a"

char* shell_read_line(void){
	int buf_size=SHELL_BUF_SIZE;
	int position = 0;
	char *buffer=malloc(sizeof(char)*buf_size);
	int c;
	if(!buffer){
		fprintf(stderr,"shell: allocation error \n");
		exit(EXIT_FAILURE);
	}
	while(1){
		c=getchar();
		if(c==EOF || c=='\n'){
			buffer[position]='\0';
			return buffer;
		}
		else {
			buffer[position]=c;
		}
		position++;

		if(position>= buf_size){
			buf_size+=SHELL_BUF_SIZE;
			buffer= realloc(buffer,buf_size);
			if(!buffer){
				fprintf(stderr,"shell: allocation error \n");
						exit(EXIT_FAILURE);
			}

		}
	}


};
char** shell_split_line(char* line){
	int buf_size= SHELL_BUF_SIZE, position=0;
	char** tokens = malloc(sizeof(char*)* buf_size);
	char* token;
	if(!tokens){
		fprintf(stderr,"shell: allocation error \n");
		exit(EXIT_FAILURE);
	}
	token =strtok(line,SHELL_DELIM);

	while(token!=NULL){
		tokens[position]=token;
		position++;
	if(position>= buf_size){
				buf_size+=SHELL_BUF_SIZE;
				tokens= realloc(tokens,buf_size*sizeof(char*));
				if(!tokens){
					fprintf(stderr,"shell: allocation error \n");
							exit(EXIT_FAILURE);
				}

			}
	token=strtok(NULL,SHELL_DELIM);
	}
 tokens[position]=NULL;
 return tokens;
};

int shell_launch(char** args){
	pid_t pid,wpid;
	int status;
	pid=fork();
	if(pid==0){
		if(execv(args[0],args ) == -1){
			perror("shell");
		}
		exit(EXIT_FAILURE);
	} else if(pid<0){
		perror("shell");
	}else{
		do{wpid=waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));

	}
	return 1;
}
int shell_help(char** args);
int shell_cd(char** args);
int shell_exit(char** args);

char* builtin_commands[]={"help","cd", "exit"};

int (*builtin_function[]) (char** )={&shell_help,&shell_cd,&shell_exit};

int shell_num_builtins(){
	return sizeof(builtin_commands)/ sizeof(char *);
}

int shell_cd(char** args){
	if(args[1] == NULL){
		fprintf(stderr,"shell:expected argument to \" cd\"\n");
	}else{
		if(chdir(args[1])!=0){
			perror("shell");
		}

	}
	return 1;

}

int shell_help(char** args){
	int i;
	printf("amaldevhari's shell \n");
	printf("Type program names and arguments, and hit enter \n");
	printf("The following commands are builtin: \n");
	for (i=0; i< shell_num_builtins();i++){
		printf(" %s\n",builtin_commands[i]);

	}
	printf("Use the man command for information on other programs.\n");

	return 1;
}

int shell_exit(char** args){
	return 0;
}

int shell_execute(char** args){
	int i;

	if(args[0] == NULL){
		return 1;
	}
	for(i=0;i<shell_num_builtins();i++){
		if( strcmp(args[0], builtin_commands[i]) == 0){
			return (builtin_function[i](args));
		}
	}
	return shell_launch(args);
}

void shell_loop(void){

	char *line;
	char **args;
	int status;
	do{
		printf("> ");
		line=shell_read_line();


		args=shell_split_line(line);

		status=shell_execute(args);


		free(line);

		free(args);

	}
	while(status);
};

int main(int argc, char **argv ){

	//start loop (shell)
	shell_loop();
	return EXIT_SUCCESS;
}
