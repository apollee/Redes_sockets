all: user fs

user: user.c
	gcc -g -Wall -o user user.c 
fs: fs.c
	gcc -g -Wall -o fs fs.c
