#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "vector.h"

typedef struct user{
	int id;
	char ip[INET_ADDRSTRLEN];
}User;

typedef struct node{
	User user; 
	struct node *next;
}* Link;

<<<<<<< HEAD
Link newLink(Link head, int id, const char* ip){
	Link x = (Link) malloc(sizeof(struct node));
 	x->user.id = id;
 	strcpy(x->user.ip, ip);
=======
Link newLink(Link head, int id, char[] ip){
	Link x = (Link) malloc(sizeof(struct Link));
 	x->user.id = id;
 	x->user.ip = ip;
>>>>>>> d01007aa409b5cec2df76ba2e3edd88b50fc86d6
 	x->next = head;
 	return x; 
}

<<<<<<< HEAD
Link insertUser(Link head, int id, const char* ip){
	Link x = newLink(head, id, ip);
	return x;
=======
Link insertUser(Link head, int id, char[] ip){
	Link = newLink(head, id, ip);
	return Link;
>>>>>>> d01007aa409b5cec2df76ba2e3edd88b50fc86d6
}

int searchID(Link head, int id){
	Link t;
	for(t = head; t != NULL;t = t->next){
		if(t->user.id == id){
			return TRUE; 
		}
	}
	return FALSE;
}

<<<<<<< HEAD
int searchIP(Link head, char* ip){
=======
int searchIP(Link head, char[] ip){
>>>>>>> d01007aa409b5cec2df76ba2e3edd88b50fc86d6
	Link t;
	for(t = head; t != NULL;t = t->next){
		if(!strcmp(t->user.ip, ip)){
			return TRUE; 
		}
	}
	return FALSE;
}

void printVector(Link head){
	for(Link t = head; t != NULL; t = t->next){
		printf("%d\n", t->user.id);
	}
}
