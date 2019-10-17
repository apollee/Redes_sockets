all: user

user: user.c parse_user.c commands_user.c directory_structure_user.c
	gcc -g -Wall -o user user.c parse_user.c commands_user.c directory_structure_user.c