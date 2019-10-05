all: user fs

user: user.c parse_user.c commands_user.c
	gcc -g -Wall -o user user.c parse_user.c commands_user.c
fs: fs.c parse_fs.c commands_fs.c 
	gcc -g -Wall -o fs fs.c parse_fs.c commands_fs.c