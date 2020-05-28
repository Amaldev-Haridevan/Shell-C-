# Shell-C-
A shell for linux written in C language.
The program invokes builtin C fucntions to execute commands.
The shell also comprises of builtin fucntions help, cd and exit

The functional flow of the shell is reading from stdin -> parsing input
into tokens -> interpreting the tokens and executing them using the system

To start the shell:

Delete the Debug folder and and invoke gcc using the following command
in the linux terminal:
"g++ main.c -o shell"

following the compilation execute the executable by using the following
command:
"./shell" 
