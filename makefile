all: user fs

user: user.c parse.c
	gcc -g -Wall -o user user.c parse.c
fs: fs.c parse.c
	gcc -g -Wall -o fs fs.c parse.c
