all: user fs

user: user.c parse.c
	gcc -o user user.c parse.c
fs: fs.c parse.c
	gcc -o fs fs.c parse.c
