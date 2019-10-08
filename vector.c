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

Link newLink(Link head, int id, const char* ip){
	Link x = (Link) malloc(sizeof(struct node));
 	x->user.id = id;
 	strcpy(x->user.ip, ip);
 	x->next = head;
 	return x; 
}

Link insertUser(Link head, int id, const char* ip){
	Link x = newLink(head, id, ip);
	return x;
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

int searchIP(Link head, char* ip){
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
