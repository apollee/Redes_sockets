all: user fs

user: user.c
	gcc -o user user.c
fs: fs.c
	gcc -o fs fs.c

