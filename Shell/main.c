/*
 * main.cpp
 *
 *  Created on: May 25, 2020
 *      Author: amaldev
 */

#include <stdio.h>
#include <stdlib.h>
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


void shell_loop(void){

	char *line;
	char **args;
	int status;
	do{
		printf("> ");
		line=shell_read_line();
		printf(line);

		args=shell_split_line(line);

		//status=shell_execute(args);

		//free(line);

		//free(args);

	}
	while(status);
};

int main(int argc, char **argv ){

	//start loop (shell)
	shell_loop();
	return EXIT_SUCCESS;
}
