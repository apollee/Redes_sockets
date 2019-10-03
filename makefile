all: user fs

user: user.c parse.c commands_user.c
	gcc -g -Wall -o user user.c parse.c commands_user.c
fs: fs.c 
	gcc -g -Wall -o fs fs.c 