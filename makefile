all: user fs

user: user.c parse_user.c commands_user.c directory_structure.c
	gcc -g -Wall -o user user.c parse_user.c commands_user.c directory_structure.c
fs: fs.c parse_fs.c commands_fs.c directory_structure.c vector.c
	gcc -g -Wall -o fs fs.c parse_fs.c commands_fs.c directory_structure.c vector.c