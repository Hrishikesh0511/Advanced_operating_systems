# Posix Shell Implementation
* Every command has it's seperate cpp file and the entire code is written in modularized fashion.
## Display Requirements:
* Everytime you open shell it shows username@systemname.
* The home directory is assumed as the directory from which you execute the program.
## Commands:
* Implemented all the given commands like cd,echo,pwd,ls,search,pinfo and their respective options which are asked.

## Foreground,Background Process:
* All the commands which are not given are executed with the help of execvp() command and background process are only possible with these ones.
## I/O redirection:
* It is assumed that spaces are given in between every command like between < and other command while running.
## History
* Implemented history command which shows the last 10 commands executed
* Also shows requested number of commands till a maximum of 20 [ history num ]
