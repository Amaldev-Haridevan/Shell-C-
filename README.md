# Shell-C-
A shell for linux written in C language.
The program invokes builtin C fucntions to execute commands.
The shell also comprises of builtin functions help, cd and exit

A clear documentation is provided inside main.c file!

The functional flow of the shell is reading from stdin -> parsing input
into tokens -> interpreting the tokens and executing them using the system -> repeat step 1

To start the shell:

make all

To clean:

make clean

following the compilation execute the executable by using the following
command:
"./shell" 


To make a desktop shortcut:
edit the shell.desktop file by executing nano "shell.desktop"
once inside edit the value of Exec to the absolute path to the execuatble generated from Makefile
 
exit and save the file

execute the command "mv shell.desktop /home/'insert your username here'/Desktop/"

To change the icon of desktop shortcut:

edit the Icon value to the absolute path to the shell.png


