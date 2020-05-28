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
#include <unistd.h>

#define SHELL_BUF_SIZE 1024
/*******************************************************************************************************
 * SHELL_BUF_SIZE defines the buffer size for the characters pulled from stdin
 ********************************************************************************************************/
#define SHELL_DELIM " \t\n\r\a"
/*******************************************************************************************************
 *  SHELL_DELIM is the string special characters that separate a meaningful token for the shell, everything
 *  separated by the special characters in SHELL_dELIM will be considered as arguments, with the first one
 *  being the command.
 *  The special characters are blank space, tab , nextline , carriage return , alert

*******************************************************************************************************/
char* shell_read_line(void){
	int buf_size=SHELL_BUF_SIZE;
	int position = 0;
	char *buffer=malloc(sizeof(char)*buf_size);
	/*******************************************************************************************************
	 * malloc assigns a memory section witht the specified bytes and returns a void pointer pointing to it.
	 ********************************************************************************************************/
	int c;
	if(!buffer){
		fprintf(stderr,"shell: allocation error \n");
		exit(EXIT_FAILURE);
		/*******************************************************************************************************
		 * if maolloc() fails send error message
		 ********************************************************************************************************/
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
/*******************************************************************************************************
 * shell_read_line(): This function pulls out characters from stdin and constructs a line with its end denoted
 * by EOF.
 ********************************************************************************************************/
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
/*******************************************************************************************************
 * shell_split_line(): This function splits the line (array of characters) that was returned by
 *  shell_read_line(), based on the SHELL_DELIM . strtok() function will split this line into tokens
 *  using the delimiter SHELL_DELIM.
 ********************************************************************************************************/
int shell_launch(char** args){
	pid_t pid,wpid;
	int status;
	pid=fork();
	if(pid==0){
		if(execvp(args[0],args ) == -1){
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
/*******************************************************************************************************
 * shell_launch(): This function uses system calls to execute a command if they are not specified
 *  in the builtin_commands variable.
 *  pid_t type stores process id, everytime a process is started it is started using the fork() function
 *  fork() creates a copy of the current running process called child. A process id value of 0 is passed
 *  to this child and a process id of the child process is given to the original.
 *
 *  The if statements:  the if statements are concurrently executed in both parent and child, so
 *  the first two if statements are meant for child and the last one is meant for parent.
 *
 *  The first if statement is executed by the child, since only child has 0 pid
 *  if the first logical condition is satisfied, using execvp() function the program to launch  and
 *   the argument for it is specified. First argument is the program. execvp() looks for this program
 *   in the $path variable of the system and executes it using the arguments provided. execvp() returns 0
 *   on successful execution. If -1 is returned it indicates failure.
 *
 *   the second if statement is only executed if the fork() process failed , fork() returns a negative pid
 *   if failed;
 *
 *   the else statement can only be executed by the parent and it waits for the child process to be finished
 *   or killed. WIFEXITED checks if the process was finished, WIFSIGNALLED checks if the process was signalled
 *   to be killed. Both returns 0 if the process is still running.
 *
 ********************************************************************************************************/
int shell_help(char** args);
int shell_cd(char** args);
int shell_exit(char** args);

/*******************************************************************************************************
 * Function prototypes for builtin commands
 ********************************************************************************************************/
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
			/*******************************************************************************************************
			 * chdir() changes the current directory
			 ********************************************************************************************************/
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
			return (*builtin_function[i])(args);
		}
	}
	return shell_launch(args);
}
/*******************************************************************************************************
 * shell_execute(): this fucntion signals the required processes depending on the tokens
 *
 * if the token is NULL this function will return
 *
 * following  the first token( command) is cross checked with the builtin functions to see if that is that
 * the user asked for.
 *
 * if not then the shell_launch() function is called;
 ********************************************************************************************************/
void shell_loop(void){

	char *line;
	char **args;
	int status;
	do{
		printf("> ");
		line=shell_read_line();

		/*******************************************************************************************************
		 * reads line.
		 ********************************************************************************************************/
		args=shell_split_line(line);
		/*******************************************************************************************************
		 * splits line into tokens
		 ********************************************************************************************************/


		status=shell_execute(args);
		/*******************************************************************************************************
		 * executes the tokens
		 ********************************************************************************************************/

		free(line);

		free(args);
		/*******************************************************************************************************
		 * frees the memory
		 ********************************************************************************************************/
	}
	while(status);
};
/*******************************************************************************************************
 * loop of the shell, only the builtin command execution shell_exit() return 0 as status.
 * The basic shell starts taking inputs, parse it into tokens and then executes them.

 ********************************************************************************************************/
int main(int argc, char **argv ){

	/*******************************************************************************************************
	 START
	 ********************************************************************************************************/
	shell_loop();
	return EXIT_SUCCESS;
	/*******************************************************************************************************
	 * EXIT MAIN
	 ********************************************************************************************************/
}
